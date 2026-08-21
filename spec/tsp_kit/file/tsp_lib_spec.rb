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
      subject(:problem) { described_class.read_file(path) }

      let(:path) { File.expand_path('../../../data/a280.tsp', __dir__) }

      it 'reads headers correctly' do
        expect(problem).to have_attributes(
          name: 'a280', comment: 'drilling problem (Ludwig)', type: 'TSP',
          dimension: 280, edge_weight_type: 'EUC_2D'
        )
      end

      it 'converts node data correctly' do
        locations = problem.get_nodes.locations

        expect([locations.shape, locations[0, true].to_a, locations[-1, true].to_a])
          .to eql [[280, 2], [288.0, 149.0], [280.0, 133.0]]
      end
    end

    context "with 'ch130.tsp'" do
      subject(:problem) { described_class.read_file(path) }

      let(:path) { File.expand_path('../../../data/ch130.tsp', __dir__) }

      it 'reads headers correctly' do
        expect(problem).to have_attributes(
          name: 'ch130', comment: '130 city problem (Churritz)', type: 'TSP',
          dimension: 130, edge_weight_type: 'EUC_2D'
        )
      end

      it 'converts node data correctly' do
        locations = problem.get_nodes.locations

        expect([locations.shape, locations[0, true].to_a, locations[-1, true].to_a])
          .to eql [[130, 2], [334.5909245845, 161.7809319139], [403.2874386776, 205.8971749407]]
      end
    end
  end
end
