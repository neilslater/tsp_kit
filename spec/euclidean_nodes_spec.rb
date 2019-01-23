require 'helpers'

describe TspKit::Nodes::Euclidean do
  describe "class methods" do
    describe "#new" do
      it "creates an object of the correct type" do
        expect( TspKit::Nodes::Euclidean.new(10, 3) ).to be_a TspKit::Nodes::Euclidean
      end

      it "does not create anything if number of nodes is out of bounds" do
        expect { TspKit::Nodes::Euclidean.new( 1, 2 ) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new( -1, 2 ) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new( 2**30, 2 ) }.to raise_error ArgumentError
      end

      it "does not create anything if number of dimensions is out of bounds" do
        expect { TspKit::Nodes::Euclidean.new( 10, 0 ) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new( 10, 1 ) }.to raise_error ArgumentError
        expect { TspKit::Nodes::Euclidean.new( 20, 500 ) }.to raise_error ArgumentError
      end

      it "creates a default locations array" do
        nodes = TspKit::Nodes::Euclidean.new( 10, 4 )
        locations = nodes.locations
        expect( locations ).to be_a NArray
        expect( locations.shape ).to eql [4, 10]
      end
    end

    describe "#from_data" do
      it "creates an object of the correct type" do
        expect( TspKit::Nodes::Euclidean.from_data(
          [
            [1.0, 2.0], [2.0, 1.0], [0.0, 0.0]
          ]
        ) ).to be_a TspKit::Nodes::Euclidean
      end

      it "stores supplied values as locations" do
        nodes = TspKit::Nodes::Euclidean.from_data(
          [
            [1.0, 2.0], [2.0, 1.0], [0.0, 0.0]
          ]
        )

        expect( nodes.num_nodes ).to eql 3
        expect( nodes.num_dims ).to eql 2
        expect( nodes.locations[0..1, 1].to_a ).to eql [2.0, 1.0]
      end
    end

    describe "#load" do
      it "instantiates correctly from a file" do
        filename = File.join(__dir__, 'test_euclidean_nodes_01.dat')
        nodes = TspKit::Nodes::Euclidean.load( filename )

        expect( nodes ).to be_a TspKit::Nodes::Euclidean

        expect( nodes.num_nodes ).to eql 3
        expect( nodes.num_dims ).to eql 2
        expect( nodes.locations[0..1, 1].to_a ).to eql [25.0, 15.0]
      end
    end
  end

  describe "instance methods" do
    let( :test_filename ) { File.join(__dir__, 'test_euclidean_nodes_01.dat') }
    subject { TspKit::Nodes::Euclidean.load( test_filename ) }

    describe "#clone" do
      it "copies everything" do
        copy = subject.clone
        expect( copy.num_nodes ).to eql subject.num_nodes
        expect( copy.num_dims ).to eql subject.num_dims
        expect( copy.locations.to_a ).to eql subject.locations.to_a
      end

      it "makes a deep copy of locations" do
        copy = subject.clone
        expect( copy.locations ).to be_narray_like subject.locations
        expect( copy.locations ).to_not be subject.locations
      end
    end

    describe "#distance_between" do
      subject { TspKit::Nodes::Euclidean.new(10, 2) }

      before :each do
        NArray.srand(12324124)
        subject.random!
      end

      it "returns 0.0 for distance between node and itself" do
        [*0..9].each do |id|
          expect( subject.distance_between( id, id ) ).to eql 0.0
        end
      end

      [2, 3, 4, 5].each do |dim|
        it "matches distances calculated in Ruby for #{dim}D locations" do
          nodes = TspKit::Nodes::Euclidean.new(10, dim)
          [*0..4].zip([*5..9]).each do |a_id, b_id|
            delta = nodes.locations[0..(dim-1), a_id] - nodes.locations[0..(dim-1), b_id]
            expected_distance = Math.sqrt((delta * delta).sum)
            expect( nodes.distance_between( a_id, b_id ) ).to be_within(1e-8).of expected_distance
          end
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
      subject { TspKit::Nodes::Euclidean.new(10, 2) }

      before :each do
        NArray.srand(12324124)
        subject.random!
      end

      it "returns a NArray of distances from a given node" do
        expect( subject.all_distances_from(0) ).to be_narray_like(
          NArray[
            0.0, 77.28642341028714, 68.9759365641762, 69.55714952267627,
            35.18914125546472, 40.87693450724964, 55.88324450929859, 56.25268255054893,
            38.69583237198266, 8.719792467772585
          ]
        )
      end
    end
  end
end
