# frozen_string_literal: true

require 'helpers'
require 'tempfile'

describe TspKit::DistanceRank do
  describe 'class methods' do
    describe '#new' do
      it 'creates an object of the correct type' do
        expect(TspKit::DistanceRank.new(10, 9)).to be_a TspKit::DistanceRank
      end

      it 'does not create anything if number of nodes is out of bounds' do
        expect { TspKit::DistanceRank.new(1, 2) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new(-1, 2) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new(2**30, 2) }.to raise_error ArgumentError
      end

      it 'does not create anything if number of dimensions is out of bounds' do
        expect { TspKit::DistanceRank.new(10, 10) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new(10, 1) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new(20, 500) }.to raise_error ArgumentError
      end

      it 'creates a default locations array' do
        nodes = TspKit::Nodes::Euclidean.new(10, 4)
        locations = nodes.locations
        expect(locations).to be_a Numo::DFloat
        expect(locations.shape).to eql [10, 4]
      end
    end

    describe '#from_data' do
      it 'creates an object of the correct type' do
        expect(TspKit::DistanceRank.from_data(
                 [
                   [1, 2], [0, 2], [0, 1]
                 ]
               )).to be_a TspKit::DistanceRank
      end

      it 'stores supplied values as closest_nodes' do
        dr = TspKit::DistanceRank.from_data(
          [
            [1, 2], [0, 2], [0, 1]
          ]
        )

        expect(dr.num_nodes).to eql 3
        expect(dr.max_rank).to eql 2
        expect(dr.closest_nodes[1, 0..1].to_a).to eql [0, 2]
      end
    end

    describe '#load' do
      it 'instantiates correctly from a file' do
        original = TspKit::DistanceRank.from_data([[1, 2], [0, 2], [0, 1]])
        dr = Tempfile.create do |file|
          original.save(file.path)
          TspKit::DistanceRank.load(file.path)
        end

        expect(dr).to be_a TspKit::DistanceRank

        expect(dr.num_nodes).to eql 3
        expect(dr.max_rank).to eql 2
        expect(dr.closest_nodes[1, 0..1].to_a).to eql [0, 2]
      end
    end
  end

  describe 'instance methods' do
    subject { TspKit::DistanceRank.from_data([[1, 2], [0, 2], [0, 1]]) }

    describe '#clone' do
      it 'copies everything' do
        copy = subject.clone
        expect(copy.num_nodes).to eql subject.num_nodes
        expect(copy.max_rank).to eql subject.max_rank
        expect(copy.closest_nodes.to_a).to eql subject.closest_nodes.to_a
      end

      it 'makes a deep copy of closest_nodes' do
        copy = subject.clone
        expect(copy.closest_nodes).to be_narray_like subject.closest_nodes
        expect(copy.closest_nodes).to_not be subject.closest_nodes
      end
    end

    describe '#resize' do
      let(:nodes) do
        Numo::NArray.srand(12_324_124)
        n = TspKit::Nodes::Euclidean.new(6, 3)
        n.random!
        n
      end
      subject { nodes.to_distance_rank(4) }

      it 'can reduce number of closest items stored' do
        expected = subject.closest_nodes[true, 0...2].dup
        subject.resize(2)
        expect(subject.closest_nodes).to be_narray_like(expected)
        expect(subject.max_rank).to be 2
      end

      it 'can increase number of closest items stored' do
        expected_prefix = subject.closest_nodes.dup
        subject.resize(5)
        expect(subject.closest_nodes[true, 0...4]).to be_narray_like(expected_prefix)
        expect(subject.closest_nodes[true, 4].to_a).to all(eql(-1))
        expect(subject.max_rank).to be 5
      end
    end

    describe '#bidirectional' do
      subject { nodes.to_distance_rank(4) }

      context 'with Euclidean nodes' do
        let(:nodes) do
          Numo::NArray.srand(12_324_124)
          n = TspKit::Nodes::Euclidean.new(6, 3)
          n.random!
          n
        end
        it 'ensures that connections are bidirectional' do
          subject.bidirectional(nodes, 2)
          expect_bidirectional_connections(subject)
          expect(subject.max_rank).to be 4
        end
      end

      # TODO: Test that we have correct support for CostMatrix with "missing links"
      context 'with CostMatrix nodes' do
        let(:nodes) do
          Numo::NArray.srand(12_324_124)
          n = TspKit::Nodes::CostMatrix.new(6)
          n.random!
          n
        end
        it 'ensures that connections are bidirectional' do
          subject.bidirectional(nodes, 4)
          expect_bidirectional_connections(subject)
          expect(subject.max_rank).to be 5
        end
      end

      context 'with one favoured node' do
        let(:nodes) do
          Numo::NArray.srand(12_324_124)
          n = TspKit::Nodes::CostMatrix.new(10)
          n.random!
          [0, 1, 2, 4, 5, 6, 7, 8, 9].each do |i|
            n.weights[3, i] = (3 + i).to_f / 10_000
            n.weights[i, 3] = (3 + i).to_f / 10_000
          end
          n
        end

        it 'still expands the closest nodes array enough to cope' do
          subject.bidirectional(nodes, 3)
          expect(subject.closest_nodes[3, true].to_a).to eql [0, 1, 2, 4, 5, 6, 7, 8, 9]
          expect_bidirectional_connections(subject)
          expect(subject.max_rank).to be 9
        end
      end
    end
  end
end
