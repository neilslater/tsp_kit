require 'helpers'

describe TspKit::Nodes::WeightMatrix do
  describe "class methods" do
    describe "#new" do
      it "creates an object of the correct type" do
        expect( TspKit::Nodes::WeightMatrix.new(10) ).to be_a TspKit::Nodes::WeightMatrix
      end

      it "does not create anything if number of nodes is out of bounds" do
        expect { TspKit::Nodes::WeightMatrix.new( 0 ) }.to raise_error ArgumentError
        expect { TspKit::Nodes::WeightMatrix.new( -10 ) }.to raise_error ArgumentError
        expect { TspKit::Nodes::WeightMatrix.new( 100000 ) }.to raise_error ArgumentError
      end

      it "creates a default weights array" do
        nodes = TspKit::Nodes::WeightMatrix.new( 10 )
        weights = nodes.weights
        expect( weights ).to be_a NArray
        expect( weights.shape ).to eql [10, 10]
      end
    end

    describe "#from_data" do
      it "creates an object of the correct type" do
        expect( TspKit::Nodes::WeightMatrix.from_data(
          [
            [0.0, 2.0, 3.0],
            [2.0, 0.0, 1.5],
            [3.0, 1.5, 0.0]
          ]
        ) ).to be_a TspKit::Nodes::WeightMatrix
      end

      it "raises an error if weights array is not symmetric" do
        expect{
          TspKit::Nodes::WeightMatrix.from_data(
            [
              [0.0, 2.0, 3.0],
              [2.0, 0.0, 1.5],
              [3.1, 1.5, 0.0]
            ]
          )
        }.to raise_error ArgumentError
      end

      it "raises an error if weights array is not right shape" do
        expect{
          TspKit::Nodes::WeightMatrix.from_data(
            [
              [0.0, 2.0, 3.0],
              [2.0, 0.0, 1.5]
            ]
          )
        }.to raise_error ArgumentError
      end

      it "stores supplied values as locations" do
        nodes = TspKit::Nodes::WeightMatrix.from_data(
          [
            [0.0, 2.0, 3.0],
            [2.0, 0.0, 1.5],
            [3.0, 1.5, 0.0]
          ]
        )

        expect( nodes.num_nodes ).to eql 3
        expect( nodes.weights[0..2, 1].to_a ).to eql [2.0, 0.0, 1.5]
      end
    end

    describe "#load" do
      it "instantiates correctly from a file" do
        filename = File.join(__dir__, 'test_weight_matrix_01.dat')
        nodes = TspKit::Nodes::WeightMatrix.load( filename )

        expect( nodes ).to be_a TspKit::Nodes::WeightMatrix

        expect( nodes.num_nodes ).to eql 3
        expect( nodes.weights[0..2, 1].to_a ).to eql [2.0, 0.0, 1.5]
      end
    end
  end

  describe "instance methods" do
    let( :test_filename ) { File.join(__dir__, 'test_weight_matrix_01.dat') }
    subject { TspKit::Nodes::WeightMatrix.load( test_filename ) }

    describe "#clone" do
      it "copies everything" do
        copy = subject.clone
        expect( copy.num_nodes ).to eql subject.num_nodes
        expect( copy.weights.to_a ).to eql subject.weights.to_a
      end

      it "makes a deep copy of weights" do
        copy = subject.clone
        expect( copy.weights ).to be_narray_like subject.weights
        expect( copy.weights ).to_not be subject.weights
      end
    end

    describe "#distance_between" do
      subject { TspKit::Nodes::WeightMatrix.new(10) }

      before :each do
        NArray.srand(12324124)
        subject.random!
      end

      it "returns expected distance between two nodes" do
        expect( subject.distance_between( 7, 8 ) ).to be_within(1e-8).of 73.640420037
      end

      it "returns 0.0 for distance between node and itself" do
        [*0..9].each do |id|
          expect( subject.distance_between( id, id ) ).to eql 0.0
        end
      end

      it "raises an error when asked for distances between non-existent nodes" do
        expect {
          subject.distance_between( 1, -1 )
        }.to raise_error ArgumentError

        expect {
          subject.distance_between( 25, 7 )
        }.to raise_error ArgumentError
      end
    end

    describe "#all_distances_from" do
      subject { TspKit::Nodes::WeightMatrix.new(10) }

      before :each do
        NArray.srand(12324124)
        subject.random!
      end

      it "returns a NArray of distances from a given node" do
        expect( subject.all_distances_from(0) ).to be_narray_like(
          NArray[
            0.0, 67.2541202375928, 43.17326578613411, 40.179165918528454, 53.31342434092694,
            19.543237018782378, 39.49916997221163, 12.314355444931753, 55.823551753117755,
            46.837376341983465
          ]
        )
      end
    end

    describe "#to_distance_rank" do
      subject { TspKit::Nodes::WeightMatrix.new(10) }

      before :each do
        NArray.srand(12324124)
        subject.random!
      end

      it "returns a sorted of array of nearest nodes by id" do
        dr = subject.to_distance_rank(7)
        expect( dr ).to be_a TspKit::DistanceRank

        expect( dr.closest_nodes ).to be_narray_like(
          NArray[
            [ 7, 5, 6, 3, 2, 9, 4 ],
            [ 6, 5, 4, 3, 8, 7, 0 ],
            [ 5, 0, 7, 8, 4, 6, 3 ],
            [ 4, 8, 5, 0, 1, 7, 6 ],
            [ 3, 9, 1, 5, 7, 0, 2 ],
            [ 3, 2, 0, 7, 6, 1, 8 ],
            [ 1, 9, 5, 7, 0, 3, 4 ],
            [ 0, 5, 6, 9, 2, 3, 4 ],
            [ 3, 5, 1, 2, 0, 9, 7 ],
            [ 6, 4, 7, 0, 5, 8, 2 ]
          ]
        )
      end
    end
  end
end
