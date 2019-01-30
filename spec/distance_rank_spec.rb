require 'helpers'

describe TspKit::DistanceRank do
  describe "class methods" do
    describe "#new" do
      it "creates an object of the correct type" do
        expect( TspKit::DistanceRank.new(10, 9) ).to be_a TspKit::DistanceRank
      end

      it "does not create anything if number of nodes is out of bounds" do
        expect { TspKit::DistanceRank.new( 1, 2 ) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new( -1, 2 ) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new( 2**30, 2 ) }.to raise_error ArgumentError
      end

      it "does not create anything if number of dimensions is out of bounds" do
        expect { TspKit::DistanceRank.new( 10, 10 ) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new( 10, 1 ) }.to raise_error ArgumentError
        expect { TspKit::DistanceRank.new( 20, 500 ) }.to raise_error ArgumentError
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
        expect( TspKit::DistanceRank.from_data(
          [
            [1, 2], [0, 2], [0, 1]
          ]
        ) ).to be_a TspKit::DistanceRank
      end

      it "stores supplied values as closest_nodes" do
        dr = TspKit::DistanceRank.from_data(
          [
            [1, 2], [0, 2], [0, 1]
          ]
        )

        expect( dr.num_nodes ).to eql 3
        expect( dr.max_rank ).to eql 2
        expect( dr.closest_nodes[0..1, 1].to_a ).to eql [0, 2]
      end
    end

    describe "#load" do
      it "instantiates correctly from a file" do
        filename = File.join(__dir__, 'test_distance_rank_01.dat')
        dr = TspKit::DistanceRank.load( filename )

        expect( dr ).to be_a TspKit::DistanceRank

        expect( dr.num_nodes ).to eql 3
        expect( dr.max_rank ).to eql 2
        expect( dr.closest_nodes[0..1, 1].to_a ).to eql [0, 2]
      end
    end
  end

  describe "instance methods" do
    let( :test_filename ) { File.join(__dir__, 'test_distance_rank_01.dat') }
    subject { TspKit::DistanceRank.load( test_filename ) }

    describe "#clone" do
      it "copies everything" do
        copy = subject.clone
        expect( copy.num_nodes ).to eql subject.num_nodes
        expect( copy.max_rank ).to eql subject.max_rank
        expect( copy.closest_nodes.to_a ).to eql subject.closest_nodes.to_a
      end

      it "makes a deep copy of closest_nodes" do
        copy = subject.clone
        expect( copy.closest_nodes ).to be_narray_like subject.closest_nodes
        expect( copy.closest_nodes ).to_not be subject.closest_nodes
      end
    end

    describe "#resize" do
      let(:nodes) { NArray.srand(12324124); n = TspKit::Nodes::Euclidean.new(6, 3); n.random!; n }
      subject { nodes.to_distance_rank(4) }

      it "can reduce number of closest items stored" do
        subject.resize(2)
        expect( subject.closest_nodes ).to be_narray_like(
          NArray[
            [ 4, 3 ],
            [ 5, 0 ],
            [ 5, 4 ],
            [ 4, 2 ],
            [ 3, 2 ],
            [ 2, 4 ]
          ]
        )
        expect( subject.max_rank ).to be 2
      end

      it "can increase number of closest items stored" do
        subject.resize(5)
        expect( subject.closest_nodes ).to be_narray_like(
          NArray[
            [ 4, 3, 1, 2, -1 ],
            [ 5, 0, 2, 4, -1 ],
            [ 5, 4, 3, 1, -1 ],
            [ 4, 2, 5, 0, -1 ],
            [ 3, 2, 5, 0, -1 ],
            [ 2, 4, 3, 1, -1 ]
          ]
        )
        expect( subject.max_rank ).to be 5
      end
    end

    describe "#bidirectional" do
      subject { nodes.to_distance_rank(4) }

      context "with Euclidean nodes" do
        let(:nodes) { NArray.srand(12324124); n = TspKit::Nodes::Euclidean.new(6, 3); n.random!; n }
        it "ensures that connections are bidirectional" do
          subject.bidirectional( nodes, 2 )
          expect( subject.closest_nodes ).to be_narray_like(
            NArray[
              [ 4, 3, 1, -1 ],
              [ 5, 0, -1, -1 ],
              [ 5, 4, 3, -1 ],
              [ 4, 2, 0, -1 ],
              [ 3, 2, 5, 0 ],
              [ 2, 4, 1, -1 ]
            ]
          )
          expect( subject.max_rank ).to be 4
        end
      end

      # TODO: Test that we have correct support for WeightMatrix with "missing links"
      context "with WeightMatrix nodes" do
        let(:nodes) { NArray.srand(12324124); n = TspKit::Nodes::WeightMatrix.new(6); n.random!; n }
        it "ensures that connections are bidirectional" do
          subject.bidirectional( nodes, 4 )
          expect( subject.closest_nodes ).to be_narray_like(
            NArray[
              [ 3, 4, 5, 2, 1 ],
              [ 4, 5, 2, 0, 3 ],
              [ 4, 3, 1, 0, -1 ],
              [ 0, 2, 5, 1, -1 ],
              [ 2, 0, 5, 1, -1 ],
              [ 4, 0, 3, 1, -1 ]
            ]
          )
          expect( subject.max_rank ).to be 5
        end
      end
    end
  end
end
