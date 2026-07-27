# frozen_string_literal: true

# tsp_kit/spec/helpers.rb
require 'tsp_kit'

# Matcher compares Numo arrays numerically
RSpec::Matchers.define :be_narray_like do |expected_narray|
  match do |given|
    @error = nil
    if !given.is_a?(Numo::NArray)
      @error = 'Wrong class.'
    elsif given.shape != expected_narray.shape
      @error = 'Shapes are different.'
    else
      d = given - expected_narray
      difference = (d * d).sum / d.size
      @error = "Numerical difference with mean square error #{difference}" if difference > 1e-9
    end
    @given = given.clone

    @expected = expected_narray.clone if @error

    !@error
  end

  failure_message do
    "Numo::NArray does not match supplied example. #{@error}
    Expected: #{@expected.inspect}
    Got: #{@given.inspect}"
  end

  failure_message_when_negated do
    "Numo::NArray is too close to unwanted example.
    Unwanted: #{@given.inspect}"
  end

  description do |_given, _expected|
    'numerically very close to example'
  end
end

def expect_bidirectional_connections(distance_rank)
  rows = distance_rank.closest_nodes.to_a
  rows.each_with_index do |connections, node_id|
    connections.reject { |other_id| other_id.negative? }.each do |other_id|
      expect(rows[other_id]).to include(node_id)
    end
  end
end
