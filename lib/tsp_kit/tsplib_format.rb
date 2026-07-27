# frozen_string_literal: true

module TspKit
  # Parsers and adapters for external file formats.
  module File
  end
end

module TspKit
  module File
    # A supported TSPLIB problem definition.
    class TspLib
      attr_reader :name, :comment, :type, :dimension, :edge_weight_type, :node_coords

      # Supported TSPLIB problem types.
      VALID_TYPES = Set['TSP']
      # Supported TSPLIB edge-weight encodings.
      VALID_EDGE_WEIGHT_TYPES = Set['EUC_2D']

      def initialize(tsplib_hash)
        @name = tsplib_hash['NAME']
        @comment = tsplib_hash['COMMENT']
        @type = validate_type(tsplib_hash['TYPE'])
        @dimension = validate_dimension(tsplib_hash['DIMENSION'])
        @edge_weight_type = validate_edge_weight_type(tsplib_hash['EDGE_WEIGHT_TYPE'])
        @node_coords = tsplib_hash['NODE_COORDS']
      end

      # Reads and validates a TSPLIB file.
      # @param file_path [String] source path
      # @return [TspLib]
      def self.read_file(file_path)
        new(file_to_tsplib_hash(file_path))
      end

      # Converts the parsed problem to TspKit nodes.
      # @return [TspKit::Nodes::Euclidean]
      def get_nodes
        generate_nodes_euc_2d
      end

      private

      def generate_nodes_euc_2d
        nodes = TspKit::Nodes::Euclidean.new(dimension, 2)
        locations = nodes.locations
        dimension.times do |i|
          locations[i, 0..1] = node_coords[i + 1]
        end
        nodes
      end

      def validate_type(possible_type)
        new_type = possible_type.strip.upcase
        raise ArgumentError, "Unrecognised type '#{possible_type}'" unless VALID_TYPES.include?(new_type)

        new_type
      end

      def validate_dimension(possible_dimension)
        new_dimension = possible_dimension.to_i
        if new_dimension < 4 || new_dimension > 1_000_000
          raise ArgumentError, "Dimension '#{possible_dimension}' out of range"
        end

        new_dimension
      end

      def validate_edge_weight_type(possible_edge_weight_type)
        new_edge_weight_type = possible_edge_weight_type.strip.upcase
        unless VALID_EDGE_WEIGHT_TYPES.include?(new_edge_weight_type)
          raise ArgumentError, "Unrecognised edge weight type '#{possible_edge_weight_type}'"
        end

        new_edge_weight_type
      end

      # Stateful line parser used while reading a TSPLIB file.
      class Parser
        attr_reader :section

        # Header names recognized by the parser.
        HEADERS = Set['NAME', 'COMMENT', 'TYPE', 'DIMENSION', 'EDGE_WEIGHT_TYPE']
        # Section markers recognized by the parser.
        SECTION_STARTS = Set['NODE_COORD_SECTION']

        def initialize
          @section = :headers
        end

        # Adds one source line to a parsed attribute hash.
        # @param text [String] source line
        # @param hash [Hash] accumulated attributes
        # @return [void]
        def add_line(text, hash)
          text = text.strip

          # Allow comments (not sure if part of TspLib spec?)
          return if text.start_with?('#')
          return if text.upcase == 'EOF'

          # New section starts?
          if SECTION_STARTS.include?(text)
            @section = text.downcase.to_sym
            return
          end

          if @section == :headers
            parse_header_line(text, hash)
          else
            parse_node_coord_line(text, hash)
          end
        end

        private

        def parse_header_line(text, hash)
          hname, hval = text.split(/\s*:\s*/, 2)
          hash[hname] = hval
        end

        def parse_node_coord_line(text, hash)
          nc = (hash['NODE_COORDS'] ||= {})
          vals = text.strip.split(/\s+/)
          id = vals.shift.to_i
          nc[id] = vals.map(&:to_f)
        end
      end

      def self.file_to_tsplib_hash(file_path)
        parser = Parser.new
        tsplib_hash = {}
        ::File.open(file_path, 'r') do |file|
          file.each_line { |line| parser.add_line(line.chomp, tsplib_hash) }
        end
        tsplib_hash
      end
      private_class_method :file_to_tsplib_hash
    end
  end
end
