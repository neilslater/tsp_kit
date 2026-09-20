# frozen_string_literal: true

# Deliberately slow, independent model of the single-pass indexed queue.
class PriorityQueueModel
  def initialize
    @queued = {}
    @settled = []
  end

  def push(id, priority, payload)
    return if @settled.include?(id)
    return if @queued.key?(id) && @queued[id].first < priority

    @queued[id] = [priority, payload]
  end

  def peek
    @queued.keys.min_by { |id| [@queued[id].first, id] } || -1
  end

  def peek_priority
    @queued.fetch(peek, [-Float::MAX, -1]).first
  end

  def peek_payload
    @queued.fetch(peek, [-Float::MAX, -1]).last
  end

  def pop
    id = peek
    if id >= 0
      @queued.delete(id)
      @settled << id
    end
    id
  end
end

# Capture every observable result, including payload replacement and empty queries.
def queue_trace(queue, operations)
  operations.map do |operation, *arguments|
    result = queue.public_send(operation, *arguments)
    [operation == :pop ? result : nil, queue.peek, queue.peek_priority, queue.peek_payload]
  end
end

def random_queue_operations(capacity, seed)
  random = Random.new(seed)
  keys = [-Float::MAX, -1e301, -17.5, -0.0, 0.0, 2.5, 1e301, Float::MAX]
  Array.new(capacity * 30) do |payload|
    if random.rand(5).zero?
      [:pop]
    else
      [:push, random.rand(capacity), keys.sample(random: random), payload]
    end
  end + Array.new(capacity + 1) { [:pop] }
end
