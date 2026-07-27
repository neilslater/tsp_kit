# frozen_string_literal: true

require 'helpers'
require 'tempfile'

describe TspKit::Nodes::Euclidean do
  describe 'class methods' do
    describe '#new' do
      it 'creates an object of the correct type' do
        expect(TspKit::Nodes::Euclidean.new(10, 3)).to be_a TspKit::Nodes::Euclidean
      end

      it 'does not create anything if number of nodes is out of bounds' do
        expect { TspKit::Nodes::Euclidean.new(1, 2) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new(-1, 2) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new(2**30, 2) }.to raise_error ArgumentError
      end

      it 'does not create anything if number of dimensions is out of bounds' do
        expect { TspKit::Nodes::Euclidean.new(10, 0) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new(10, 1) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new(20, 500) }.to raise_error ArgumentError
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
        expect(TspKit::Nodes::Euclidean.from_data(
                 [
                   [1.0, 2.0], [2.0, 1.0], [0.0, 0.0]
                 ]
               )).to be_a TspKit::Nodes::Euclidean
      end

      it 'stores supplied values as locations' do
        nodes = TspKit::Nodes::Euclidean.from_data(
          [
            [1.0, 2.0], [2.0, 1.0], [0.0, 0.0]
          ]
        )

        expect(nodes.num_nodes).to eql 3
        expect(nodes.num_dims).to eql 2
        expect(nodes.locations[0..1, 1].to_a).to eql [2.0, 1.0]
      end
    end

    describe '#load' do
      it 'instantiates correctly from a file' do
        original = TspKit::Nodes::Euclidean.from_data([[1, 2], [25, 15], [0, 0]])
        nodes = Tempfile.create do |file|
          original.save(file.path)
          TspKit::Nodes::Euclidean.load(file.path)
        end

        expect(nodes).to be_a TspKit::Nodes::Euclidean

        expect(nodes.num_nodes).to eql 3
        expect(nodes.num_dims).to eql 2
        expect(nodes.locations[1, 0..1].to_a).to eql [25.0, 15.0]
      end
    end
  end

  describe 'instance methods' do
    subject { TspKit::Nodes::Euclidean.from_data([[1, 2], [25, 15], [0, 0]]) }

    describe '#clone' do
      it 'copies everything' do
        copy = subject.clone
        expect(copy.num_nodes).to eql subject.num_nodes
        expect(copy.num_dims).to eql subject.num_dims
        expect(copy.locations.to_a).to eql subject.locations.to_a
      end

      it 'makes a deep copy of locations' do
        copy = subject.clone
        expect(copy.locations).to be_narray_like subject.locations
        expect(copy.locations).to_not be subject.locations
      end
    end

    describe '#distance_between' do
      subject { TspKit::Nodes::Euclidean.new(10, 2) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns 0.0 for distance between node and itself' do
        [*0..9].each do |id|
          expect(subject.distance_between(id, id)).to eql 0.0
        end
      end

      [2, 3, 4, 5].each do |dim|
        it "matches distances calculated in Ruby for #{dim}D locations" do
          nodes = TspKit::Nodes::Euclidean.new(10, dim)
          [*0..4].zip([*5..9]).each do |a_id, b_id|
              delta = nodes.locations[a_id, 0..(dim - 1)] - nodes.locations[b_id, 0..(dim - 1)]
            expected_distance = Math.sqrt((delta * delta).sum)
            expect(nodes.distance_between(a_id, b_id)).to be_within(1e-8).of expected_distance
          end
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
      subject { TspKit::Nodes::Euclidean.new(10, 2) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns a Numo::DFloat of distances from a given node' do
        expected = Numo::NMath.sqrt(((subject.locations - subject.locations[0, true])**2).sum(axis: 1))
        expect(subject.all_distances_from(0)).to be_narray_like(expected)
      end
    end

    describe '#to_cost_matrix' do
      subject { TspKit::Nodes::Euclidean.new(6, 3) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns a weight matrix of all distances' do
        cm = subject.to_cost_matrix
        expect(cm).to be_a TspKit::Nodes::CostMatrix

        expect(cm.weights).to be_narray_like(cm.weights.transpose)
        6.times do |from|
          6.times do |to|
            expect(cm.weights[from, to]).to be_within(1e-9).of(subject.distance_between(from, to))
          end
        end
      end
    end

    describe '#to_distance_rank' do
      subject { TspKit::Nodes::Euclidean.new(6, 3) }

      before :each do
        Numo::NArray.srand(12_324_124)
        subject.random!
      end

      it 'returns a sorted of array of nearest nodes by id' do
        dr = subject.to_distance_rank(4)
        expect(dr).to be_a TspKit::DistanceRank

        6.times do |node_id|
          expected = (0...6).reject { |id| id == node_id }
                           .sort_by { |id| subject.distance_between(node_id, id) }.first(4)
          expect(dr.closest_nodes[node_id, true].to_a).to eql expected
        end
      end
    end
  end
end
