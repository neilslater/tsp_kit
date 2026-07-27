# frozen_string_literal: true

module TspKit
  class Nodes
    class Euclidean
      # Randomises the locations using Numo's random number generator.
      # Use Numo::NArray.srand to set repeatable seed values.
      # @param [Range,Array<Range>] limits defaults 0..100
      # @return [Numo::DFloat] altered locations table
      def random!(limits = 0.0..100.0)
        limits = [limits] * num_dims unless limits.is_a? Array

        if limits.count != num_dims || !limits.all? { |l| l.is_a?(Range) && l.first.is_a?(Numeric) }
          raise ArgumentError, "Cannot apply randomiser limits as supplied: #{limits.inspect}"
        end

        offsets = limits.map(&:first)
        sizes = limits.map { |l| l.last - l.first }

        locations[] = locations.rand * sizes + offsets
      end
    end
  end
end

module TspKit
  class Nodes
    class CostMatrix
      # Randomises the weights using Numo's random number generator.
      # Use Numo::NArray.srand to set repeatable seed values.
      # @param [max] limits defaults to 100.0
      # @return [Numo::DFloat] altered weights table
      def random!(max = 100.0)
        weights[] = weights.rand * (max / 2)
        weights[] = weights + weights.transpose(1, 0)
        [*0...num_nodes].each { |i| weights[i, i] = 0.0 }
        validate
        weights
      end
    end
  end
end
