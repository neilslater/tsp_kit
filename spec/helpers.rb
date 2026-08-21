# frozen_string_literal: true

# tsp_kit/spec/helpers.rb
require 'tsp_kit'
require 'tempfile'

def marshal_round_trip(object)
  Tempfile.create do |file|
    object.save(file.path)
    object.class.load(file.path)
  end
end

def expect_argument_errors(*operations)
  errors = operations.map do |operation|
    operation.call
    nil
  rescue ArgumentError => e
    e.class
  end
  expect(errors).to all(eq(ArgumentError))
end

def push_all(queue, entries)
  entries.each { |entry| queue.push(*entry) }
end

def pop_pairs(queue, count)
  Array.new(count) { [queue.peek_payload, queue.pop] }
end

def pop_entries(queue, count)
  Array.new(count) do
    [queue.peek, queue.peek_priority, queue.peek_payload, queue.pop]
  end
end

def expected_pop_entries(entries)
  entries.map { |id, priority, payload| [id, priority, payload, id] }
end

def execute_queue_steps(queue, steps)
  steps.map do |inputs, expected|
    push_all(queue, inputs)
    pop_entries(queue, expected.length)
  end
end

def exercise_queue_batches(queue, batches, final_ids)
  collected = batches.flat_map do |ids, pop_count, payload|
    ids.each { |id| queue.push(id, rand, payload) }
    Array.new(pop_count) { queue.pop }
  end
  final_ids.each { |id| queue.push(id, rand, batches.length + 1) }
  [collected.sort, queue.pop]
end

def ruby_distances(nodes, pairs)
  pairs.map do |from, to|
    delta = nodes.locations[from, true] - nodes.locations[to, true]
    Math.sqrt((delta * delta).sum)
  end
end

def expect_cost_matrix_for(matrix, nodes)
  expected = Numo::DFloat.cast(Array.new(nodes.num_nodes) do |from|
    Array.new(nodes.num_nodes) { |to| nodes.distance_between(from, to) }
  end)
  weights_matcher = be_narray_like(expected).and be_narray_like(matrix.weights.transpose)
  expect(matrix).to be_a(TspKit::Nodes::CostMatrix).and have_attributes(weights: weights_matcher)
end

def expected_neighbor_rows(nodes, count)
  nodes.num_nodes.times.map do |node_id|
    (0...nodes.num_nodes).reject { |id| id == node_id }
                         .sort_by { |id| nodes.distance_between(node_id, id) }.first(count)
  end
end

def shuffled_values(generator, seed, size = 10)
  generator.srand(seed)
  values = Numo::Int32.zeros(size).seq
  generator.shuffle_narray(values)
  values.to_a
end

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
    connections.reject(&:negative?).each do |other_id|
      expect(rows[other_id]).to include(node_id)
    end
  end
end
