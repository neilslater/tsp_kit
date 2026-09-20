# frozen_string_literal: true

require 'helpers'
require 'support/priority_queue_model'

describe TspKit::Utils::PriorityQueue do
  subject(:queue) { described_class.new(5) }

  describe 'input validation' do
    [0, -1, -2_147_483_648].each do |capacity|
      it "rejects nonpositive capacity #{capacity}" do
        expect { described_class.new(capacity) }.to raise_error(ArgumentError, /capacity/)
      end
    end

    [2**31, -(2**31) - 1, 2**100].each do |capacity|
      it "rejects capacity outside the native integer range: #{capacity}" do
        expect { described_class.new(capacity) }.to raise_error(RangeError)
      end
    end

    [-1, 5, 2_147_483_647].each do |id|
      it "rejects invalid ID #{id} without changing the queue", :aggregate_failures do
        queue.push(2, 3.0, 7)
        expect { queue.push(id, -4.0, 8) }.to raise_error(ArgumentError, /ID/)
        expect(pop_entries(queue, 2)).to eq [[2, 3.0, 7, 2], [-1, -Float::MAX, -1, -1]]
      end
    end

    [Float::NAN, Float::INFINITY, -Float::INFINITY].each do |priority|
      it "rejects #{priority} for unseen, queued, and settled IDs without mutation" do
        queue.push(0, 1.0, 0)
        queue.pop
        queue.push(1, 2.0, 7)
        expect_argument_errors(*[0, 1, 2].map { |id| -> { queue.push(id, priority, 8) } })
        expect(pop_entries(queue, 2)).to eq [[1, 2.0, 7, 1], [-1, -Float::MAX, -1, -1]]
      end
    end

    it 'rejects unrepresentable IDs and payloads before mutation', :aggregate_failures do
      queue.push(1, 2.0, 7)
      expect { queue.push(2**100, 0.0, 8) }.to raise_error(RangeError)
      expect { queue.push(1, 0.0, 2**100) }.to raise_error(RangeError)
      expect(pop_entries(queue, 1)).to eq [[1, 2.0, 7, 1]]
    end

    it 'rejects reinitialization without losing the existing heap', :aggregate_failures do
      queue.push(1, 2.0, 7)
      expect { queue.send(:initialize, 10) }.to raise_error(ArgumentError, /already initialized/)
      expect(pop_entries(queue, 1)).to eq [[1, 2.0, 7, 1]]
    end

    it 'rejects copying over an initialized queue without mutation', :aggregate_failures do
      queue.push(1, 2.0, 7)
      expect { queue.send(:initialize_copy, described_class.new(2)) }
        .to raise_error(ArgumentError, /already initialized/)
      expect(pop_entries(queue, 1)).to eq [[1, 2.0, 7, 1]]
    end

    it 'rejects pushes into an uninitialized allocation' do
      expect { described_class.allocate.push(0, 1.0, 0) }.to raise_error(ArgumentError, /ID/)
    end
  end

  describe 'finite keys and settled IDs' do
    it 'decreases and replaces a repeated enormous key without reinserting its node' do
      push_all(queue, [[0, Float::MAX, 0], [1, 1e302, 1], [0, Float::MAX, 2], [1, 1e301, 3]])
      expect(pop_entries(queue, 3)).to eq [[1, 1e301, 3, 1], [0, Float::MAX, 2, 0],
                                           [-1, -Float::MAX, -1, -1]]
    end

    it 'ignores settled updates even at the former negative sentinel' do
      queue.push(1, -Float::MAX, 3)
      queue.pop
      queue.push(1, -Float::MAX, 4)
      expect(pop_entries(queue, 1)).to eq [[-1, -Float::MAX, -1, -1]]
    end

    it 'orders equal keys by ID and keeps the newest equal-key payload' do
      push_all(queue, [[4, 2.0, 4], [2, 2.0, 2], [1, 3.0, 1], [1, 2.0, 8], [2, 2.0, 9]])
      expect(pop_entries(queue, 3)).to eq [[1, 2.0, 8, 1], [2, 2.0, 9, 2], [4, 2.0, 4, 4]]
    end

    [1, 2, 3, 10, 101].each do |capacity|
      [1234, 5678, 90_123].each do |seed|
        it "matches the model through mixed operations and draining (size #{capacity}, seed #{seed})" do
          operations = random_queue_operations(capacity, seed)
          actual = queue_trace(described_class.new(capacity), operations)
          expect(actual).to eq queue_trace(PriorityQueueModel.new, operations)
        end
      end
    end

    [10, 11].each do |capacity|
      it "drains a full heap with #{capacity - 1} root children" do
        operations = Array.new(capacity) { |id| [:push, id, id.to_f, id] }
        operations += Array.new(capacity + 1) { [:pop] }
        expect(queue_trace(described_class.new(capacity), operations))
          .to eq queue_trace(PriorityQueueModel.new, operations)
      end
    end
  end

  describe 'copying mixed states' do
    let(:initial) do
      [[:push, 0, -Float::MAX, 0], [:push, 1, Float::MAX, 1], [:push, 2, 1e301, 2],
       [:push, 3, 2.5, 3], [:pop]]
    end

    let(:continuations) do
      copy_steps = [[:push, 0, -Float::MAX, 9], [:push, 1, Float::MAX, 8], [:push, 4, -1e301, 4]]
      [copy_steps + random_queue_operations(5, 123), random_queue_operations(5, 456)]
    end

    %i[clone dup].each do |method|
      it "preserves mixed states with independent updates through #{method}" do
        queue_trace(queue, initial)
        copy = queue.public_send(method)
        actual = [queue_trace(copy, continuations.first), queue_trace(queue, continuations.last)]
        expected = continuations.map { |steps| queue_trace(PriorityQueueModel.new, initial + steps).drop(initial.size) }
        expect(actual).to eq expected
      end
    end
  end
end
