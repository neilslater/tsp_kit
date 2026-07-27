# frozen_string_literal: true

require 'numo/narray/alt'

module TspKit
  module MarshalSupport
    # @!visibility private
    def _dump(*_ignored)
      Marshal.dump to_h
    end

    def save(filename)
      ::File.open(filename, 'wb') { |file| Marshal.dump(self, file) }
    end

    def self.included(base)
      base.extend(ClassMethods)
    end

    module ClassMethods
      # @!visibility private
      def _load(buf)
        h = Marshal.load buf
        from_h h
      end

      def load(filename)
        ::File.open(filename, 'rb') { |file| Marshal.load(file) }
      end
    end
  end
end

module TspKit
  class Nodes
    class Euclidean
      include TspKit::MarshalSupport
      # @!visibility private
      # Adds support for Marshal, via to_h and from_h methods
      def to_h
        { locations: locations }
      end

      # @!visibility private
      # Constructs a TspKit::Nodes::Euclidean from hash description. Used internally to support Marshal.
      # @param [Hash] h Keys are :locations
      # @return [TspKit::Nodes::Euclidean] new object
      def self.from_h(h)
        TspKit::Nodes::Euclidean.from_data(h[:locations])
      end
    end
  end
end

module TspKit
  class Nodes
    class CostMatrix
      include TspKit::MarshalSupport
      # @!visibility private
      # Adds support for Marshal, via to_h and from_h methods
      def to_h
        { weights: weights }
      end

      # @!visibility private
      # Constructs a TspKit::Nodes::CostMatrix from hash description. Used internally to support Marshal.
      # @param [Hash] h Keys are :weights
      # @return [TspKit::Nodes::CostMatrix] new object
      def self.from_h(h)
        TspKit::Nodes::CostMatrix.from_data(h[:weights])
      end
    end
  end
end

module TspKit
  class DistanceRank
    include TspKit::MarshalSupport
    # @!visibility private
    # Adds support for Marshal, via to_h and from_h methods
    def to_h
      { closest_nodes: closest_nodes }
    end

    # @!visibility private
    # Constructs a TspKit::DistanceRankfrom hash description. Used internally to support Marshal.
    # @param [Hash] h Keys are :closest_nodes
    # @return [TspKit::DistanceRank] new object
    def self.from_h(h)
      TspKit::DistanceRank.from_data(h[:closest_nodes])
    end
  end
end
