require "narray"

# TspKit adds support for Marshal to NArray.
# Code originally from http://blade.nagaokaut.ac.jp/cgi-bin/scat.rb/ruby/ruby-talk/194510
class NArray
  def _dump *ignored
    Marshal.dump :typecode => typecode, :shape => shape, :data => to_s
  end
  def self._load buf
    h = Marshal.load buf
    typecode = h[:typecode]
    shape = h[:shape]
    data = h[:data]
    to_na data, typecode, *shape
  end

  def save filename
    File.open( filename, 'wb' ) { |file| Marshal.dump( self, file ) }
  end

  def self.load filename
    File.open( filename, 'rb' ) { |file| Marshal.load(file) }
  end
end

module TspKit::MarshalSupport
  # @!visibility private
  def _dump *ignored
    Marshal.dump to_h
  end

  def save filename
    File.open( filename, 'wb' ) { |file| Marshal.dump( self, file ) }
  end

  def self.included(base)
    base.extend(ClassMethods)
  end

  module ClassMethods
    # @!visibility private
    def _load buf
      h = Marshal.load buf
      from_h h
    end

    def load filename
      File.open( filename, 'rb' ) { |file| Marshal.load(file) }
    end
  end
end

class TspKit::Nodes::Euclidean
  include TspKit::MarshalSupport
  # @!visibility private
  # Adds support for Marshal, via to_h and from_h methods
  def to_h
    Hash[
      :locations => self.locations
    ]
  end

  # @!visibility private
  # Constructs a TspKit::Nodes::Euclidean from hash description. Used internally to support Marshal.
  # @param [Hash] h Keys are :locations
  # @return [TspKit::Nodes::Euclidean] new object
  def self.from_h h
    Santa2018::Cities.from_data( h[:locations] )
  end
end
