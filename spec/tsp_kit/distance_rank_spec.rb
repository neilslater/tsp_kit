# frozen_string_literal: true

require 'helpers'

describe TspKit::DistanceRank do
  subject(:distance_rank) { described_class.from_data([[1, 2], [0, 2], [0, 1]]) }

  describe 'class methods' do
    describe '#new' do
      it 'creates an object of the correct type' do
        expect(described_class.new(10, 9)).to be_a described_class
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
          -> { described_class.new(10, 10) },
          -> { described_class.new(10, 1) },
          -> { described_class.new(20, 500) }
        )
      end

      it 'creates a default locations array' do
        locations = TspKit::Nodes::Euclidean.new(10, 4).locations

        expect(locations).to be_a(Numo::DFloat).and have_attributes(shape: [10, 4])
      end
    end

    describe '#from_data' do
      let(:closest_nodes) { [[1, 2], [0, 2], [0, 1]] }

      it 'creates an object of the correct type' do
        expect(described_class.from_data(closest_nodes)).to be_a described_class
      end

      it 'stores supplied values as closest_nodes' do
        rank = described_class.from_data(closest_nodes)

        expect([rank.num_nodes, rank.max_rank, rank.closest_nodes[1, 0..1].to_a])
          .to eql [3, 2, [0, 2]]
      end
    end

    describe '#load' do
      it 'instantiates correctly from a file' do
        original = described_class.from_data([[1, 2], [0, 2], [0, 1]])
        rank = marshal_round_trip(original)

        expect([rank.class, rank.num_nodes, rank.max_rank, rank.closest_nodes[1, 0..1].to_a])
          .to eql [described_class, 3, 2, [0, 2]]
      end
    end
  end

  describe '#clone' do
    it 'copies everything' do
      copy = distance_rank.clone

      expect([copy.num_nodes, copy.max_rank, copy.closest_nodes.to_a])
        .to eql [distance_rank.num_nodes, distance_rank.max_rank, distance_rank.closest_nodes.to_a]
    end

    it 'makes a deep copy of closest_nodes' do
      copy = distance_rank.clone

      expect([copy.closest_nodes.to_a, copy.closest_nodes.equal?(distance_rank.closest_nodes)])
        .to eql [distance_rank.closest_nodes.to_a, false]
    end
  end

  describe '#resize' do
    subject(:distance_rank) { nodes.to_distance_rank(4) }

    let(:nodes) do
      Numo::NArray.srand(12_324_124)
      nodes = TspKit::Nodes::Euclidean.new(6, 3)
      nodes.random!
      nodes
    end

    it 'can reduce number of closest items stored' do
      expected = distance_rank.closest_nodes[true, 0...2].dup
      distance_rank.resize(2)

      expect([distance_rank.closest_nodes.to_a, distance_rank.max_rank])
        .to eql [expected.to_a, 2]
    end

    it 'can increase number of closest items stored' do
      expected_prefix = distance_rank.closest_nodes.dup
      distance_rank.resize(5)

      expect([distance_rank.closest_nodes[true, 0...4].to_a,
              distance_rank.closest_nodes[true, 4].to_a, distance_rank.max_rank])
        .to eql [expected_prefix.to_a, Array.new(6, -1), 5]
    end
  end

  describe '#bidirectional' do
    subject(:distance_rank) { nodes.to_distance_rank(4) }

    context 'with Euclidean nodes' do
      let(:nodes) do
        Numo::NArray.srand(12_324_124)
        nodes = TspKit::Nodes::Euclidean.new(6, 3)
        nodes.random!
        nodes
      end

      it 'ensures that connections are bidirectional' do
        distance_rank.bidirectional(nodes, 2)
        expect_bidirectional_connections(distance_rank)
        expect(distance_rank.max_rank).to be 4
      end
    end

    # TODO: Test that we have correct support for CostMatrix with "missing links"
    context 'with CostMatrix nodes' do
      let(:nodes) do
        Numo::NArray.srand(12_324_124)
        nodes = TspKit::Nodes::CostMatrix.new(6)
        nodes.random!
        nodes
      end

      it 'ensures that connections are bidirectional' do
        distance_rank.bidirectional(nodes, 4)
        expect_bidirectional_connections(distance_rank)
        expect(distance_rank.max_rank).to be 5
      end
    end

    context 'with one favoured node' do
      let(:nodes) do
        Numo::NArray.srand(12_324_124)
        nodes = TspKit::Nodes::CostMatrix.new(10)
        nodes.random!
        [0, 1, 2, 4, 5, 6, 7, 8, 9].each do |index|
          nodes.weights[3, index] = (3 + index).to_f / 10_000
          nodes.weights[index, 3] = (3 + index).to_f / 10_000
        end
        nodes
      end

      it 'still expands the closest nodes array enough to cope' do
        distance_rank.bidirectional(nodes, 3)
        expect_bidirectional_connections(distance_rank)
        expect([distance_rank.closest_nodes[3, true].to_a, distance_rank.max_rank])
          .to eql [[0, 1, 2, 4, 5, 6, 7, 8, 9], 9]
      end
    end
  end
end
