# frozen_string_literal: true

require 'helpers'

describe TspKit::Utils::PriorityQueue do
  describe 'push and pop' do
    subject(:queue) { described_class.new(10) }

    it 'returns -1 for pop by default' do
      expect(queue.pop).to be(-1)
    end

    it 'can push an id and priority, then get the id back' do
      push_all(queue, [[7, 1.3, 0]])

      expect(pop_pairs(queue, 2)).to eql [[0, 7], [-1, -1]]
    end

    it 'can push three ids and priorities, then get them back in priority order' do
      push_all(queue, [[7, 1.3, 4], [6, 1.8, 3], [5, 0.7, 2]])

      expect(pop_pairs(queue, 4)).to eql [[2, 5], [4, 7], [3, 6], [-1, -1]]
    end

    context 'when exercising a longer sequence' do
      subject(:queue) { described_class.new(100) }

      it 'can push twenty ids and priorities, then get them back in priority order' do
        inputs = [
          [0, 8.9, 14], [1, 7.9, 9], [2, 20.0, 4], [3, 14.0, 16], [4, 59.3, 18],
          [5, 97.5, 0], [6, 42.9, 7], [7, 3.1, 2], [8, 82.7, 17], [9, 98.0, 16],
          [10, 60.4, 0], [11, 32.7, 11], [12, 99.4, 19], [13, 31.4, 17],
          [14, 33.0, 2], [15, 23.8, 14], [16, 99.7, 19], [17, 8.0, 13],
          [18, 34.4, 2], [19, 35.7, 4]
        ]
        expected = [
          [7, 3.1, 2], [1, 7.9, 9], [17, 8.0, 13], [0, 8.9, 14], [3, 14.0, 16],
          [2, 20.0, 4], [15, 23.8, 14], [13, 31.4, 17], [11, 32.7, 11],
          [14, 33.0, 2], [18, 34.4, 2], [19, 35.7, 4], [6, 42.9, 7], [4, 59.3, 18],
          [10, 60.4, 0], [8, 82.7, 17], [5, 97.5, 0], [9, 98.0, 16],
          [12, 99.4, 19], [16, 99.7, 19]
        ]
        push_all(queue, inputs)

        expect(pop_entries(queue, expected.length)).to eql expected_pop_entries(expected)
      end

      it 'can push forty ids and priorities with repeats, split, then get them back in priority order' do
        steps = [
          [
            [
              [0, 8.9, 14], [1, 7.9, 9], [2, 20.0, 4], [3, 14.0, 16], [4, 77.3, 11],
              [6, 45.0, 6], [7, 3.1, 2], [8, 82.7, 17], [9, 99.0, 19],
              [10, 62.4, 1], [11, 32.7, 11], [12, 99.4, 19], [13, 31.4, 17],
              [14, 33.0, 2], [15, 23.8, 14], [16, 99.9, 19], [17, 8.0, 13],
              [18, 34.4, 2]
            ],
            [
              [7, 3.1, 2], [1, 7.9, 9], [17, 8.0, 13], [0, 8.9, 14], [3, 14.0, 16],
              [2, 20.0, 4], [15, 23.8, 14], [13, 31.4, 17], [11, 32.7, 11],
              [14, 33.0, 2], [18, 34.4, 2]
            ]
          ],
          [
            [
              [1, 14.5, 9], [2, 10.0, 4], [4, 59.3, 18], [5, 97.5, 0], [6, 42.9, 7],
              [8, 82.7, 17], [9, 98.0, 16], [10, 60.4, 0], [11, 30.7, 11],
              [12, 99.4, 19], [13, 31.4, 17], [15, 23.8, 14], [16, 99.7, 19],
              [17, 8.0, 13], [18, 34.4, 2], [19, 35.7, 4]
            ],
            [
              [19, 35.7, 4], [6, 42.9, 7], [4, 59.3, 18], [10, 60.4, 0],
              [8, 82.7, 17], [5, 97.5, 0], [9, 98.0, 16], [12, 99.4, 19],
              [16, 99.7, 19]
            ]
          ]
        ]
        actual = execute_queue_steps(queue, steps)
        expected = steps.map { |_inputs, entries| expected_pop_entries(entries) }

        expect(actual).to eql(expected)
      end

      it 'can push forty ids and priorities with repeats, then get them back in priority order' do
        inputs = [
          [0, 9.1, 12], [1, 7.9, 9], [2, 20.1, 3], [3, 14.0, 16], [4, 59.4, 19],
          [5, 97.5, 0], [6, 47.9, 9], [0, 8.9, 14], [1, 8.9, 6], [2, 20.0, 4],
          [3, 15.0, 15], [4, 59.3, 18], [5, 97.8, 1], [6, 42.9, 7], [7, 3.2, 1],
          [8, 82.7, 17], [9, 98.0, 16], [10, 60.5, 1], [11, 32.7, 11],
          [12, 99.6, 20], [7, 3.1, 2], [8, 82.8, 18], [9, 98.9, 12],
          [10, 60.4, 0], [11, 32.9, 12], [12, 99.4, 19], [13, 31.6, 19],
          [14, 33.0, 2], [15, 23.9, 10], [16, 99.7, 19], [17, 9.0, 10],
          [18, 34.4, 2], [19, 35.9, 5], [13, 31.4, 17], [14, 33.2, 4],
          [15, 23.8, 14], [16, 99.9, 20], [17, 8.0, 13], [18, 34.5, 1],
          [19, 35.7, 4]
        ]
        expected = [
          [7, 3.1, 2], [1, 7.9, 9], [17, 8.0, 13], [0, 8.9, 14], [3, 14.0, 16],
          [2, 20.0, 4], [15, 23.8, 14], [13, 31.4, 17], [11, 32.7, 11],
          [14, 33.0, 2], [18, 34.4, 2], [19, 35.7, 4], [6, 42.9, 7], [4, 59.3, 18],
          [10, 60.4, 0], [8, 82.7, 17], [5, 97.5, 0], [9, 98.0, 16],
          [12, 99.4, 19], [16, 99.7, 19]
        ]
        push_all(queue, inputs)

        expect(pop_entries(queue, expected.length)).to eql expected_pop_entries(expected)
      end
    end

    it 'can re-prioritise to higher priority' do
      push_all(queue, [[7, 1.3, 4], [6, 1.8, 3], [5, 0.7, 2], [6, 0.3, 8]])

      expect(pop_pairs(queue, 4)).to eql [[8, 6], [2, 5], [4, 7], [-1, -1]]
    end

    it "doesn't re-prioritise to lower priority" do
      push_all(queue, [[7, 1.3, 4], [6, 1.8, 5], [5, 0.7, 2], [7, 2.8, 8]])

      expect(pop_pairs(queue, 4)).to eql [[2, 5], [4, 7], [5, 6], [-1, -1]]
    end

    it 'handles mixed push and pop' do
      push_all(queue, [[7, 1.3, 0], [6, 1.8, 1], [5, 2.9, 2]])
      first_results = pop_pairs(queue, 2)
      push_all(queue, [[9, 3.3, 9], [8, 0.5, 8]])

      expect(first_results + pop_pairs(queue, 4))
        .to eql [[0, 7], [1, 6], [8, 8], [2, 5], [9, 9], [-1, -1]]
    end

    it 'ignores new priorities for completed items' do
      push_all(queue, [[7, 1.3, 5], [6, 1.8, 4], [5, 2.9, 2]])
      first_results = pop_pairs(queue, 2)
      push_all(queue, [[7, 0.3, 9], [9, 3.3, 1], [8, 0.5, 12], [6, 0.8, 9]])

      expect(first_results + pop_pairs(queue, 4))
        .to eql [[5, 7], [4, 6], [12, 8], [2, 5], [1, 9], [-1, -1]]
    end

    [124_781, 12_414_325, 141_251, 431_251, 431_252, 431_253, 431_254, 431_255].each do |seed|
      it "nothing bad happens if we try to over-fill the queue (seed #{seed})" do
        srand(seed)
        result = exercise_queue_batches(queue, [[0...10, 5, 1], [0...10, 5, 2]], 0...10)

        expect(result).to eql [(0...10).to_a, -1]
      end

      it "nothing bad happens if we try to over-fill a large queue (seed #{seed})" do
        srand(seed)
        large_queue = described_class.new(200)
        batches = [[0...100, 40, 1], [50...150, 40, 2], [100...200, 40, 3], [100...200, 80, 4]]

        expect(exercise_queue_batches(large_queue, batches, 0...10)).to eql [(0...200).to_a, -1]
      end
    end
  end
end
