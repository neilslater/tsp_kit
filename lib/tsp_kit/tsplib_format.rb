require 'set'

module TspKit::File
end

class TspKit::File::TspLib
  attr_reader :name, :comment, :type, :dimension, :edge_weight_type, :node_coords

  VALID_TYPES = Set['TSP']
  VALID_EDGE_WEIGHT_TYPES = Set['EUC_2D']

  def initialize tsplib_hash
    @name = tsplib_hash['NAME']
    @comment = tsplib_hash['COMMENT']
    @type = validate_type(tsplib_hash['TYPE'])
    @dimension = validate_dimension(tsplib_hash['DIMENSION'])
    @edge_weight_type = validate_edge_weight_type(tsplib_hash['EDGE_WEIGHT_TYPE'])
    @node_coords = tsplib_hash['NODE_COORDS']
  end

  def self.read_file file_path
    self.new(file_to_tsplib_hash(file_path))
  end

  def get_nodes
    case edge_weight_type
    when 'EUC_2D'
      generate_nodes_euc_2d
    end
  end

  private

  def generate_nodes_euc_2d
    nodes = TspKit::Nodes::Euclidean.new( dimension, 2 )
    locations = nodes.locations
    dimension.times do |i|
      coords = node_coords[i+1]
      locations[0..1, i] = coords
    end
    nodes
  end

  def validate_type possible_type
    new_type = possible_type.strip.upcase
    unless VALID_TYPES.include?(new_type)
      raise ArgumentError, "Unrecognised type '#{possible_type}'"
    end
    new_type
  end

  def validate_dimension possible_dimension
    new_dimension = possible_dimension.to_i
    if new_dimension < 4 || new_dimension > 1_000_000
      raise ArgumentError, "Dimension '#{possible_dimension}' out of range"
    end
    new_dimension
  end

  def validate_edge_weight_type possible_edge_weight_type
    new_edge_weight_type = possible_edge_weight_type.strip.upcase
    unless VALID_EDGE_WEIGHT_TYPES.include?(new_edge_weight_type)
      raise ArgumentError, "Unrecognised edge weight type '#{possible_edge_weight_type}'"
    end
    new_edge_weight_type
  end

  class Parser
    attr_reader :section

    HEADERS = Set['NAME', 'COMMENT', 'TYPE', 'DIMENSION', 'EDGE_WEIGHT_TYPE']
    SECTION_STARTS = Set['NODE_COORD_SECTION']

    def initialize
      @section = :headers
    end

    def add_line text, hash
      text = text.strip

      # Allow comments (not sure if part of TspLib spec?)
      return if text.start_with?('#')
      return if text.upcase == 'EOF'

      # New section starts?
      if SECTION_STARTS.include?(text)
        @section = text.downcase.to_sym
        return
      end

      case @section
      when :headers
        parse_header_line(text, hash)
      when :node_coord_section
        parse_node_coord_line(text, hash)
      end
    end

    private

    def parse_header_line text, hash
      hname, hval = text.split(/\s*:\s*/, 2)
      hash[hname] = hval
    end

    def parse_node_coord_line text, hash
      nc = (hash['NODE_COORDS'] ||= {})
      vals = text.strip.split(/\s+/).map(&:to_i) # TSPLIB uses integers throughout
      id = vals.shift
      nc[id] = vals
    end
  end

  def self.file_to_tsplib_hash file_path
    parser = Parser.new
    tsplib_hash = {}
    File.open(file_path, "r").each_line do |line|
      parser.add_line(line.chomp, tsplib_hash)
    end
    tsplib_hash
  end
end
