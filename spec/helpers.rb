# frozen_string_literal: true

# tsp_kit/spec/helpers.rb
require 'tsp_kit'

# Matcher compares NArrays numerically
RSpec::Matchers.define :be_narray_like do |expected_narray|
  match do |given|
    @error = nil
    if !given.is_a?(NArray)
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
    "NArray does not match supplied example. #{@error}
    Expected: #{@expected.inspect}
    Got: #{@given.inspect}"
  end

  failure_message_when_negated do
    "NArray is too close to unwanted example.
    Unwanted: #{@given.inspect}"
  end

  description do |_given, _expected|
    'numerically very close to example'
  end
end
