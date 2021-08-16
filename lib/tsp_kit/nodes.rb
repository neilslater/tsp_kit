# frozen_string_literal: true

module TspKit
  module Nodes
    class Euclidean
      # Randomises the locations, useful for tests and random challenges. Note this uses NArray's random
      # so use NArray.srand to set seed values for repeat tests.
      # @param [Range,Array<Range>] limits defaults 0..100
      # @return [NArray] altered locations table
      def random!(limits = 0.0..100.0)
        limits = [limits] * num_dims unless limits.is_a? Array

        if limits.count != num_dims || !limits.all? { |l| l.is_a?(Range) && l.first.is_a?(Numeric) }
          raise ArgumentError, "Cannot apply randomiser limits as supplied: #{limits.inspect}"
        end

        offsets = limits.map(&:first)
        sizes = limits.map { |l| l.last - l.first }

        locations.random!.mul!(sizes).add!(offsets)
      end
    end
  end
end

module TspKit
  module Nodes
    class CostMatrix
      # Randomises the weights matrix, useful for tests and random challenges. Note this uses NArray's random
      # so use NArray.srand to set seed values for repeat tests.
      # @param [max] limits defaults to 100.0
      # @return [NArray] altered locations table
      def random!(max = 100.0)
        weights.random!.mul!(max / 2)
        weights.add!(weights.transpose(1, 0))
        [*0...num_nodes].each { |i| weights[i, i] = 0.0 }
        validate
        weights
      end
    end
  end
end
