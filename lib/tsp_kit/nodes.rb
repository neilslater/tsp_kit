# frozen_string_literal: true

module TspKit
  class Nodes
    # Adds Ruby conveniences to native Euclidean node collections.
    class Euclidean
      # Randomises the locations using Numo's random number generator.
      # Use Numo::NArray.srand to set repeatable seed values.
      # @param [Range,Array<Range>] limits defaults 0..100
      # @return [Numo::DFloat] altered locations table
      def random!(limits = 0.0..100.0)
        limits = normalize_limits(limits)
        offsets = limits.map(&:first)
        sizes = limits.map { |limit| limit.last - limit.first }

        locations[] = (locations.rand * sizes) + offsets
      end

      private

      def normalize_limits(limits)
        limits = Array.new(num_dims, limits) unless limits.is_a?(Array)
        return limits if valid_limits?(limits)

        raise ArgumentError, "Cannot apply randomiser limits as supplied: #{limits.inspect}"
      end

      def valid_limits?(limits)
        limits.count == num_dims && limits.all? do |limit|
          limit.is_a?(Range) && limit.first.is_a?(Numeric)
        end
      end
    end

    # Adds Ruby conveniences to native cost-matrix node collections.
    class CostMatrix
      # Randomises the weights using Numo's random number generator.
      # Use Numo::NArray.srand to set repeatable seed values.
      # @param max [Numeric] exclusive upper bound for generated weights
      # @return [Numo::DFloat] altered weights table
      def random!(max = 100.0)
        random_weights = weights.rand * (max / 2)
        weights[] = random_weights + random_weights.transpose(1, 0)
        num_nodes.times { |index| weights[index, index] = 0.0 }
        validate
        weights
      end
    end
  end
end
