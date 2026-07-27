# frozen_string_literal: true

require 'helpers'

describe TspKit::File::TspLib do
  describe '.read_file' do
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
        locations = subject.get_nodes.locations
        expect(locations.shape).to eql [280, 2]
        expect(locations[0, true].to_a).to eql [288.0, 149.0]
        expect(locations[-1, true].to_a).to eql [280.0, 133.0]
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
        locations = subject.get_nodes.locations
        expect(locations.shape).to eql [130, 2]
        expect(locations[0, true].to_a).to eql [334.5909245845, 161.7809319139]
        expect(locations[-1, true].to_a).to eql [403.2874386776, 205.8971749407]
      end
    end
  end
end
