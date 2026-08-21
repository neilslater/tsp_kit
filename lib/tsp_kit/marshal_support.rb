# frozen_string_literal: true

require 'numo/narray/alt'

module TspKit
  # Adds trusted-file Marshal persistence to Numo-backed native objects.
  module MarshalSupport
    # @!visibility private
    def _dump(*_ignored)
      Marshal.dump to_h
    end

    # Saves this object using Ruby's Marshal format.
    #
    # Only load files from trusted sources; Marshal is not safe for untrusted
    # input.
    # @param filename [String] destination path
    # @return [void]
    def save(filename)
      ::File.open(filename, 'wb') { |file| Marshal.dump(self, file) }
    end

    # Extends including classes with the matching load helpers.
    # @param base [Class] including class
    # @return [void]
    def self.included(base)
      base.extend(ClassMethods)
    end

    # Class-level reconstruction helpers for Marshal persistence.
    module ClassMethods
      # @!visibility private
      def _load(buffer)
        attributes = Marshal.load buffer
        from_h attributes
      end

      # Loads an object previously written by {MarshalSupport#save}.
      #
      # Only load files from trusted sources; Marshal can execute attacker-
      # controlled object hooks.
      # @param filename [String] trusted source path
      # @return [Object] reconstructed TspKit object
      def load(filename)
        ::File.open(filename, 'rb') { |file| Marshal.load(file) }
      end
    end
  end

  class Nodes
    # Marshal persistence for Euclidean node collections.
    class Euclidean
      include TspKit::MarshalSupport

      # @!visibility private
      # Adds support for Marshal, via to_h and from_h methods
      def to_h
        { locations: locations }
      end

      # @!visibility private
      # Constructs a TspKit::Nodes::Euclidean from hash description. Used internally to support Marshal.
      # @param attributes [Hash] keys are `:locations`
      # @return [TspKit::Nodes::Euclidean] new object
      def self.from_h(attributes)
        TspKit::Nodes::Euclidean.from_data(attributes[:locations])
      end
    end

    # Marshal persistence for cost-matrix node collections.
    class CostMatrix
      include TspKit::MarshalSupport

      # @!visibility private
      # Adds support for Marshal, via to_h and from_h methods
      def to_h
        { weights: weights }
      end

      # @!visibility private
      # Constructs a TspKit::Nodes::CostMatrix from hash description. Used internally to support Marshal.
      # @param attributes [Hash] keys are `:weights`
      # @return [TspKit::Nodes::CostMatrix] new object
      def self.from_h(attributes)
        TspKit::Nodes::CostMatrix.from_data(attributes[:weights])
      end
    end
  end

  # Marshal persistence for nearest-neighbour indexes.
  class DistanceRank
    include TspKit::MarshalSupport

    # @!visibility private
    # Adds support for Marshal, via to_h and from_h methods
    def to_h
      { closest_nodes: closest_nodes }
    end

    # @!visibility private
    # Constructs a TspKit::DistanceRankfrom hash description. Used internally to support Marshal.
    # @param attributes [Hash] keys are `:closest_nodes`
    # @return [TspKit::DistanceRank] new object
    def self.from_h(attributes)
      TspKit::DistanceRank.from_data(attributes[:closest_nodes])
    end
  end
end
