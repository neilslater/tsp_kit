# frozen_string_literal: true

require 'helpers'

describe TspKit::File::TspLib do
  describe '.new' do
    let(:valid_attributes) do
      {
        'NAME' => 'square',
        'COMMENT' => 'fixture',
        'TYPE' => 'TSP',
        'DIMENSION' => '4',
        'EDGE_WEIGHT_TYPE' => 'EUC_2D',
        'NODE_COORDS' => { 1 => [0.0, 0.0], 2 => [1.0, 0.0], 3 => [1.0, 1.0], 4 => [0.0, 1.0] }
      }
    end

    it 'rejects unsupported problem types' do
      expect { described_class.new(valid_attributes.merge('TYPE' => 'CVRP')) }.to raise_error(ArgumentError)
    end

    it 'rejects dimensions outside the supported range' do
      expect { described_class.new(valid_attributes.merge('DIMENSION' => '3')) }.to raise_error(ArgumentError)
    end

    it 'rejects unsupported edge weight types' do
      attributes = valid_attributes.merge('EDGE_WEIGHT_TYPE' => 'GEO')
      expect { described_class.new(attributes) }.to raise_error(ArgumentError)
    end
  end

  describe TspKit::File::TspLib::Parser do
    it 'ignores comments and EOF markers' do
      parsed = {}
      parser = described_class.new

      parser.add_line('# comment', parsed)
      parser.add_line('EOF', parsed)

      expect(parsed).to be_empty
    end
  end

  describe '.read_file' do
    context "with 'a280.tsp'" do
      subject { described_class.read_file(path) }

      let(:path) { File.join(__dir__, '..', 'data', 'a280.tsp') }

      it 'reads headers correctly' do
        expect(subject.name).to eql 'a280'
        expect(subject.comment).to eql 'drilling problem (Ludwig)'
        expect(subject.type).to eql 'TSP'
        expect(subject.dimension).to be 280
        expect(subject.edge_weight_type).to eql 'EUC_2D'
      end

      it 'converts node data correctly' do
        locations = subject.get_nodes.locations
        expect(locations.shape).to eql [280, 2]
        expect(locations[0, true].to_a).to eql [288.0, 149.0]
        expect(locations[-1, true].to_a).to eql [280.0, 133.0]
      end
    end

    context "with 'ch130.tsp'" do
      subject { described_class.read_file(path) }

      let(:path) { File.join(__dir__, '..', 'data', 'ch130.tsp') }

      it 'reads headers correctly' do
        expect(subject.name).to eql 'ch130'
        expect(subject.comment).to eql '130 city problem (Churritz)'
        expect(subject.type).to eql 'TSP'
        expect(subject.dimension).to be 130
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
