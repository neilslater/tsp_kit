require 'helpers'

describe TspKit::Nodes::Euclidean do
  # We only do 2D for now, but the data structure can support more dimensions
  describe "2D" do
    describe "#new" do
      it "creates an object of the correct type" do
        expect( TspKit::Nodes::Euclidean.new(10, 2) ).to be_a TspKit::Nodes::Euclidean
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
        nodes = TspKit::Nodes::Euclidean.new( 10, 2 )
        locations = nodes.locations
        expect( locations ).to be_a NArray
        expect( locations.shape ).to eql [2, 10]
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
end
