# frozen_string_literal: true

require 'helpers'

describe TspKit::Nodes::Euclidean do
  subject(:nodes) { described_class.from_data([[1, 2], [25, 15], [0, 0]]) }

  describe 'class methods' do
    describe '#new' do
      it 'creates an object of the correct type' do
        expect(described_class.new(10, 3)).to be_a described_class
      end

      it 'does not create anything if number of nodes is out of bounds' do
        expect_argument_errors(
          -> { described_class.new(1, 2) },
          -> { described_class.new(-1, 2) },
          -> { described_class.new(2**30, 2) }
        )
      end

      it 'does not create anything if number of dimensions is out of bounds' do
        expect_argument_errors(
          -> { described_class.new(10, 0) },
          -> { described_class.new(10, 1) },
          -> { described_class.new(20, 500) }
        )
      end

      it 'creates a default locations array' do
        locations = described_class.new(10, 4).locations

        expect(locations).to be_a(Numo::DFloat).and have_attributes(shape: [10, 4])
      end
    end

    describe '#from_data' do
      let(:locations) { [[1.0, 2.0], [2.0, 1.0], [0.0, 0.0]] }

      it 'creates an object of the correct type' do
        expect(described_class.from_data(locations)).to be_a described_class
      end

      it 'stores supplied values as locations' do
        nodes = described_class.from_data(locations)

        expect([nodes.num_nodes, nodes.num_dims, nodes.locations[0..1, 1].to_a])
          .to eql [3, 2, [2.0, 1.0]]
      end
    end

    describe '#load' do
      it 'instantiates correctly from a file' do
        original = described_class.from_data([[1, 2], [25, 15], [0, 0]])
        nodes = marshal_round_trip(original)

        expect([nodes.class, nodes.num_nodes, nodes.num_dims, nodes.locations[1, 0..1].to_a])
          .to eql [described_class, 3, 2, [25.0, 15.0]]
      end
    end
  end

  describe '#random!' do
    it 'accepts one range per dimension' do
      nodes.random!([10.0..20.0, -5.0..5.0])
      minima = nodes.locations.min(axis: 0).to_a

      expect(minima).to match [be_between(10.0, 20.0), be_between(-5.0, 5.0)]
    end

    it 'rejects the wrong number of ranges' do
      expect { nodes.random!([0.0..1.0]) }.to raise_error(ArgumentError)
    end

    it 'rejects ranges whose lower bound is not numeric' do
      expect { nodes.random!(['a'..'z', 0.0..1.0]) }.to raise_error(ArgumentError)
    end
  end

  describe '#clone' do
    it 'copies everything' do
      copy = nodes.clone

      expect([copy.num_nodes, copy.num_dims, copy.locations.to_a])
        .to eql [nodes.num_nodes, nodes.num_dims, nodes.locations.to_a]
    end

    it 'makes a deep copy of locations' do
      copy = nodes.clone

      expect([copy.locations.to_a, copy.locations.equal?(nodes.locations)])
        .to eql [nodes.locations.to_a, false]
    end
  end

  describe '#distance_between' do
    subject(:nodes) { described_class.new(10, 2) }

    before do
      Numo::NArray.srand(12_324_124)
      nodes.random!
    end

    it 'returns 0.0 for distance between node and itself' do
      expect(10.times.map { |id| nodes.distance_between(id, id) }).to all(be(0.0))
    end

    [2, 3, 4, 5].each do |dimensions|
      it "matches distances calculated in Ruby for #{dimensions}D locations" do
        nodes = described_class.new(10, dimensions)
        pairs = (0..4).zip(5..9)
        actual = pairs.map { |from, to| nodes.distance_between(from, to) }

        expect(actual).to match(ruby_distances(nodes, pairs).map { |value| be_within(1e-8).of(value) })
      end
    end

    it 'raises an error when asked for distances between non-existent nodes' do
      expect_argument_errors(
        -> { nodes.distance_between(1, -1) },
        -> { nodes.distance_between(25, 7) }
      )
    end
  end

  describe '#all_distances_from' do
    subject(:nodes) { described_class.new(10, 2) }

    before do
      Numo::NArray.srand(12_324_124)
      nodes.random!
    end

    it 'returns a Numo::DFloat of distances from a given node' do
      expected = Numo::NMath.sqrt(((nodes.locations - nodes.locations[0, true])**2).sum(axis: 1))

      expect(nodes.all_distances_from(0)).to be_narray_like(expected)
    end
  end

  describe '#to_cost_matrix' do
    subject(:nodes) { described_class.new(6, 3) }

    before do
      Numo::NArray.srand(12_324_124)
      nodes.random!
    end

    it 'returns a weight matrix of all distances' do
      expect_cost_matrix_for(nodes.to_cost_matrix, nodes)
    end
  end

  describe '#to_distance_rank' do
    subject(:nodes) { described_class.new(6, 3) }

    before do
      Numo::NArray.srand(12_324_124)
      nodes.random!
    end

    it 'returns a sorted of array of nearest nodes by id' do
      distance_rank = nodes.to_distance_rank(4)

      expect([distance_rank.class, distance_rank.closest_nodes.to_a])
        .to eql [TspKit::DistanceRank, expected_neighbor_rows(nodes, 4)]
    end
  end
end
