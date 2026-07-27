# frozen_string_literal: true

require 'helpers'
require 'tempfile'

describe TspKit::Nodes::CostMatrix do
  describe 'class methods' do
    describe '#new' do
      it 'creates an object of the correct type' do
        expect(TspKit::Nodes::CostMatrix.new(10)).to be_a TspKit::Nodes::CostMatrix
      end

      it 'does not create anything if number of nodes is out of bounds' do
        expect { TspKit::Nodes::CostMatrix.new(0) }.to raise_error ArgumentError
        expect { TspKit::Nodes::CostMatrix.new(-10) }.to raise_error ArgumentError
        expect { TspKit::Nodes::CostMatrix.new(100_000) }.to raise_error ArgumentError
      end

      it 'creates a default weights array' do
        nodes = TspKit::Nodes::CostMatrix.new(10)
        weights = nodes.weights
        expect(weights).to be_a Numo::DFloat
        expect(weights.shape).to eql [10, 10]
      end
    end

    describe '#from_data' do
      it 'creates an object of the correct type' do
        expect(TspKit::Nodes::CostMatrix.from_data(
                 [
                   [0.0, 2.0, 3.0],
                   [2.0, 0.0, 1.5],
                   [3.0, 1.5, 0.0]
                 ]
               )).to be_a TspKit::Nodes::CostMatrix
      end

      it 'raises an error if weights array is not symmetric' do
        expect do
          TspKit::Nodes::CostMatrix.from_data(
            [
              [0.0, 2.0, 3.0],
              [2.0, 0.0, 1.5],
              [3.1, 1.5, 0.0]
            ]
          )
        end.to raise_error ArgumentError
      end

      it 'raises an error if weights array is not right shape' do
        expect do
          TspKit::Nodes::CostMatrix.from_data(
            [
              [0.0, 2.0, 3.0],
              [2.0, 0.0, 1.5]
            ]
          )
        end.to raise_error ArgumentError
      end

      it 'stores supplied values as locations' do
        nodes = TspKit::Nodes::CostMatrix.from_data(
          [
            [0.0, 2.0, 3.0],
            [2.0, 0.0, 1.5],
            [3.0, 1.5, 0.0]
          ]
        )

        expect(nodes.num_nodes).to eql 3
        expect(nodes.weights[0..2, 1].to_a).to eql [2.0, 0.0, 1.5]
      end
    end

    describe '#load' do
      it 'instantiates correctly from a file' do
        original = TspKit::Nodes::CostMatrix.from_data(
          [[0, 2, 3], [2, 0, 1.5], [3, 1.5, 0]]
        )
        nodes = Tempfile.create do |file|
          original.save(file.path)
          TspKit::Nodes::CostMatrix.load(file.path)
        end

        expect(nodes).to be_a TspKit::Nodes::CostMatrix

        expect(nodes.num_nodes).to eql 3
        expect(nodes.weights[0..2, 1].to_a).to eql [2.0, 0.0, 1.5]
      end
    end
  end

  describe 'instance methods' do
    subject do
      TspKit::Nodes::CostMatrix.from_data([[0, 2, 3], [2, 0, 1.5], [3, 1.5, 0]])
    end

    describe '#clone' do
      it 'copies everything' do
        copy = subject.clone
        expect(copy.num_nodes).to eql subject.num_nodes
        expect(copy.weights.to_a).to eql subject.weights.to_a
      end

      it 'makes a deep copy of weights' do
        copy = subject.clone
        expect(copy.weights).to be_narray_like subject.weights
        expect(copy.weights).to_not be subject.weights
      end
    end

    describe '#distance_between' do
      subject { TspKit::Nodes::CostMatrix.new(10) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns expected distance between two nodes' do
        expect(subject.distance_between(7, 8)).to eql subject.weights[7, 8]
      end

      it 'returns 0.0 for distance between node and itself' do
        [*0..9].each do |id|
          expect(subject.distance_between(id, id)).to eql 0.0
        end
      end

      it 'raises an error when asked for distances between non-existent nodes' do
        expect do
          subject.distance_between(1, -1)
        end.to raise_error ArgumentError

        expect do
          subject.distance_between(25, 7)
        end.to raise_error ArgumentError
      end
    end

    describe '#all_distances_from' do
      subject { TspKit::Nodes::CostMatrix.new(10) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns a Numo::DFloat of distances from a given node' do
        expect(subject.all_distances_from(0)).to be_narray_like(subject.weights[0, true])
      end
    end

    describe '#to_distance_rank' do
      subject { TspKit::Nodes::CostMatrix.new(10) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns a sorted of array of nearest nodes by id' do
        dr = subject.to_distance_rank(7)
        expect(dr).to be_a TspKit::DistanceRank

        10.times do |node_id|
          expected = (0...10).reject { |id| id == node_id }
                             .sort_by { |id| subject.weights[node_id, id] }.first(7)
          expect(dr.closest_nodes[node_id, true].to_a).to eql expected
        end
      end
    end
  end
end
