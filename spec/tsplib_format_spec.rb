require 'helpers'

describe TspKit::File::TspLib do
  describe ".read_file" do
    context "'a280.tsp'" do
      let(:path) { File.join(__dir__, '..', 'data', 'a280.tsp') }
      subject { TspKit::File::TspLib.read_file(path) }

      it 'reads headers correctly' do
        expect(subject.name).to eql 'a280'
        expect(subject.comment).to eql 'drilling problem (Ludwig)'
        expect(subject.type).to eql 'TSP'
        expect(subject.dimension).to eql 280
        expect(subject.edge_weight_type).to eql 'EUC_2D'
      end

      it 'converts node data correctly' do
        expected_nodes = TspKit::Nodes::Euclidean.load('data/a280_nodes.dat')
        got_nodes = subject.get_nodes
        expect(got_nodes.locations).to be_narray_like expected_nodes.locations
      end
    end

    context "'ch130.tsp'" do
      let(:path) { File.join(__dir__, '..', 'data', 'ch130.tsp') }
      subject { TspKit::File::TspLib.read_file(path) }

      it 'reads headers correctly' do
        expect(subject.name).to eql 'ch130'
        expect(subject.comment).to eql '130 city problem (Churritz)'
        expect(subject.type).to eql 'TSP'
        expect(subject.dimension).to eql 130
        expect(subject.edge_weight_type).to eql 'EUC_2D'
      end

      it 'converts node data correctly' do
        expected_nodes = TspKit::Nodes::Euclidean.load('data/ch130_nodes.dat')
        got_nodes = subject.get_nodes
        expect(got_nodes.locations).to be_narray_like expected_nodes.locations
      end
    end
  end
end
