# frozen_string_literal: true

# Native data structures and algorithms for travelling salesman problems.
module TspKit
  # Seeds TspKit's native random-number generator.
  # @param seed [Integer] 32-bit seed
  # @return [nil]
  def self.srand(seed); end

  # Seeds TspKit's native random-number generator from multiple words.
  # @param seeds [Array<Integer>] up to 640 32-bit seed values
  # @return [nil]
  def self.srand_array(seeds); end

  # Returns a uniformly distributed random value.
  # @return [Float] value in the range 0.0...1.0
  def self.rand; end

  # Returns a normally distributed random value.
  # @return [Float]
  def self.randn; end

  # Returns a half-normal distributed integer.
  # @param maximum [Integer] exclusive upper bound
  # @return [Integer]
  def self.halfnorm_int(maximum); end

  # Returns a uniformly distributed integer.
  # @param maximum [Integer] exclusive upper bound
  # @return [Integer]
  def self.random_int_up_to(maximum); end

  # Collections of TSP nodes represented by Numo arrays.
  class Nodes
    # Nodes represented by Euclidean coordinates.
    class Euclidean
      # @!method initialize(num_nodes, num_dims)
      #   Creates an empty Euclidean node collection.
      #   @param num_nodes [Integer] number of nodes
      #   @param num_dims [Integer] number of coordinate dimensions
      # @!method self.from_data(locations)
      #   @param locations [Numo::DFloat, Array<Array<Numeric>>] coordinate rows
      #   @return [Euclidean]
      # @!attribute [r] num_nodes
      #   @return [Integer]
      # @!attribute [r] num_dims
      #   @return [Integer]
      # @!attribute [r] locations
      #   @return [Numo::DFloat] row-major coordinates
      # @!method distance_between(node_a_id, node_b_id)
      #   @return [Float]
      # @!method all_distances_from(node_id)
      #   @return [Numo::DFloat]
      # @!method to_cost_matrix
      #   @return [CostMatrix]
      # @!method to_distance_rank(max_rank)
      #   @return [DistanceRank]
    end

    # Nodes represented by a complete symmetric cost matrix.
    class CostMatrix
      # @!method initialize(num_nodes)
      #   Creates an empty square cost matrix.
      #   @param num_nodes [Integer] number of nodes
      # @!method self.from_data(weights)
      #   @param weights [Numo::DFloat, Array<Array<Numeric>>] square weight matrix
      #   @return [CostMatrix]
      # @!attribute [r] num_nodes
      #   @return [Integer]
      # @!attribute [r] weights
      #   @return [Numo::DFloat]
      # @!method validate
      #   @return [true]
      #   @raise [ArgumentError] if weights are invalid
      # @!method distance_between(node_a_id, node_b_id)
      #   @return [Float]
      # @!method all_distances_from(node_id)
      #   @return [Numo::DFloat]
      # @!method to_distance_rank(max_rank)
      #   @return [DistanceRank]
    end
  end

  # Ranked nearest-neighbour indexes for every node.
  class DistanceRank
    # @!method initialize(num_nodes, max_rank)
    #   Creates an empty nearest-neighbour index.
    # @!method self.from_data(closest_nodes)
    #   @return [DistanceRank]
    # @!attribute [r] num_nodes
    #   @return [Integer]
    # @!attribute [r] max_rank
    #   @return [Integer]
    # @!attribute [r] closest_nodes
    #   @return [Numo::Int32]
    # @!method resize(max_rank)
    #   @return [DistanceRank] self
    # @!method bidirectional(nodes, max_rank)
    #   @return [DistanceRank] self
  end

  # A mutable tour represented by native index arrays.
  class Solution
    # @!method initialize(num_nodes)
    #   Creates an identity-order tour.
    # @!attribute [r] num_nodes
    #   @return [Integer]
    # @!attribute [r] ids
    #   @return [Numo::Int32]
    # @!attribute [r] node_idx
    #   @return [Numo::Int32]
  end

  # Native one-tree workspace used by solvers.
  class OneTree
    # @!method initialize(num_nodes)
    #   Creates an empty one-tree workspace.
    # @!attribute [r] num_nodes
    #   @return [Integer]
    # @!attribute [r] node_penalties
    #   @return [Numo::DFloat]
    # @!attribute [r] node_ids
    #   @return [Numo::Int32]
    # @!attribute [r] parents
    #   @return [Numo::Int32]
  end

  # Native greedy-solver workspace.
  class GreedySolver
    # @!method initialize(num_nodes)
    #   Creates a greedy-solver workspace.
    # @!attribute [r] num_nodes
    #   @return [Integer]
    # @!attribute [r] max_section_id
    #   @return [Integer]
  end

  # Internal utility data structures.
  module Utils
    # Fixed-capacity native priority queue keyed by integer identifiers.
    class PriorityQueue
      # @!method initialize(size)
      #   Creates an empty queue with fixed capacity.
      # @!attribute [r] pq_size
      #   @return [Integer]
      # @!attribute [r] heap_root
      #   @return [Integer]
      # @!method push(id, priority, payload)
      #   @return [PriorityQueue] self
      # @!method pop
      #   @return [Integer] highest-priority identifier
      # @!method peek
      #   @return [Integer] highest-priority identifier
      # @!method peek_priority
      #   @return [Float]
      # @!method peek_payload
      #   @return [Integer]
    end
  end
end
