class TspKit::Nodes::Euclidean

  # Randomises the locations, useful for tests and random challenges. Note this uses NArray's random
  # so use NArray.srand to set seed values for repeat tests.
  # @param [Range,Array<Range>] limits defaults 0..100
  # @return [NArray] altered locations table
  def random! limits = 0.0..100.0
    unless limits.is_a? Array
      limits = [limits] * num_dims
    end

    if limits.count != num_dims || ! limits.all? { |l| l.is_a?(Range) && l.first.is_a?(Numeric) }
      raise ArgumentError, "Cannot apply randomiser limits as supplied: #{limits.inspect}"
    end

    offsets = limits.map { |l| l.first }
    sizes = limits.map { |l| l.last - l.first }

    locations.random!.mul!(sizes).add!(offsets)
  end
end

class TspKit::Nodes::WeightMatrix
  # Randomises the weights matrix, useful for tests and random challenges. Note this uses NArray's random
  # so use NArray.srand to set seed values for repeat tests.
  # @param [max] limits defaults to 100.0
  # @return [NArray] altered locations table
  def random! max = 100.0
    weights.random!.mul!(max/2)
    weights.add!( weights.transpose(1,0) )
    [*0...num_nodes].each { |i| weights[i, i] = 0.0 }
    validate
    weights
  end
end
