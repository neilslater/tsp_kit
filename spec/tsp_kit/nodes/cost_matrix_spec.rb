# frozen_string_literal: true

require 'helpers'

describe TspKit::Nodes::CostMatrix do
  describe 'class methods' do
    describe '#new' do
      it 'creates an object of the correct type' do
        expect(described_class.new(10)).to be_a described_class
      end

      it 'does not create anything if number of nodes is out of bounds' do
        expect_argument_errors(
          -> { described_class.new(0) },
          -> { described_class.new(-10) },
          -> { described_class.new(100_000) }
        )
      end

      it 'creates a default weights array' do
        weights = described_class.new(10).weights

        expect(weights).to be_a(Numo::DFloat).and have_attributes(shape: [10, 10])
      end
    end

    describe '#from_data' do
      let(:valid_weights) do
        [[0.0, 2.0, 3.0], [2.0, 0.0, 1.5], [3.0, 1.5, 0.0]]
      end

      it 'creates an object of the correct type' do
        expect(described_class.from_data(valid_weights)).to be_a described_class
      end

      it 'raises an error if weights array is not symmetric' do
        asymmetric = [[0.0, 2.0, 3.0], [2.0, 0.0, 1.5], [3.1, 1.5, 0.0]]

        expect { described_class.from_data(asymmetric) }.to raise_error ArgumentError
      end

      it 'raises an error if weights array is not right shape' do
        expect { described_class.from_data(valid_weights.first(2)) }.to raise_error ArgumentError
      end

      it 'stores supplied values as locations' do
        nodes = described_class.from_data(valid_weights)

        expect([nodes.num_nodes, nodes.weights[0..2, 1].to_a]).to eql [3, [2.0, 0.0, 1.5]]
      end
    end

    describe '#load' do
      it 'instantiates correctly from a file' do
        original = described_class.from_data([[0, 2, 3], [2, 0, 1.5], [3, 1.5, 0]])
        nodes = marshal_round_trip(original)

        expect([nodes.class, nodes.num_nodes, nodes.weights[0..2, 1].to_a])
          .to eql [described_class, 3, [2.0, 0.0, 1.5]]
      end
    end
  end

  describe 'instance methods' do
    subject(:nodes) do
      described_class.from_data([[0, 2, 3], [2, 0, 1.5], [3, 1.5, 0]])
    end

    describe '#clone' do
      it 'copies everything' do
        copy = nodes.clone

        expect([copy.num_nodes, copy.weights.to_a]).to eql [nodes.num_nodes, nodes.weights.to_a]
      end

      it 'makes a deep copy of weights' do
        copy = nodes.clone

        expect([copy.weights.to_a, copy.weights.equal?(nodes.weights)])
          .to eql [nodes.weights.to_a, false]
      end
    end

    describe '#distance_between' do
      subject(:nodes) { described_class.new(10) }

      before do
        Numo::NArray.srand(12_324_124)
        nodes.random!
      end

      it 'returns expected distance between two nodes' do
        expect(nodes.distance_between(7, 8)).to eql nodes.weights[7, 8]
      end

      it 'returns 0.0 for distance between node and itself' do
        expect(10.times.map { |id| nodes.distance_between(id, id) }).to all(be(0.0))
      end

      it 'raises an error when asked for distances between non-existent nodes' do
        expect_argument_errors(
          -> { nodes.distance_between(1, -1) },
          -> { nodes.distance_between(25, 7) }
        )
      end
    end

    describe '#all_distances_from' do
      subject(:nodes) { described_class.new(10) }

      before do
        Numo::NArray.srand(12_324_124)
        nodes.random!
      end

      it 'returns a Numo::DFloat of distances from a given node' do
        expect(nodes.all_distances_from(0)).to be_narray_like(nodes.weights[0, true])
      end
    end

    describe '#to_distance_rank' do
      subject(:nodes) { described_class.new(10) }

      before do
        Numo::NArray.srand(12_324_124)
        nodes.random!
      end

      it 'returns a sorted of array of nearest nodes by id' do
        distance_rank = nodes.to_distance_rank(7)

        expect([distance_rank.class, distance_rank.closest_nodes.to_a])
          .to eql [TspKit::DistanceRank, expected_neighbor_rows(nodes, 7)]
      end
    end
  end
end
