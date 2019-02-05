require 'helpers'

describe TspKit::Utils::PriorityQueue do
  describe "push and pop" do
    subject { TspKit::Utils::PriorityQueue.new(10) }

    it "returns -1 for pop by default" do
      expect(subject.pop).to eql(-1)
    end

    it "can push an id and priority, then get the id back" do
      subject.push(7, 1.3, 0)

      expect(subject.peek_payload).to eql(0)
      expect(subject.pop).to eql(7)

      expect(subject.peek_payload).to eql(-1)
      expect(subject.pop).to eql(-1)
    end

    it "can push three ids and priorities, then get them back in priority order" do
      subject.push(7, 1.3, 4)
      subject.push(6, 1.8, 3)
      subject.push(5, 0.7, 2)

      expect(subject.peek_payload).to eql(2)
      expect(subject.pop).to eql(5)

      expect(subject.peek_payload).to eql(4)
      expect(subject.pop).to eql(7)

      expect(subject.peek_payload).to eql(3)
      expect(subject.pop).to eql(6)

      expect(subject.peek_payload).to eql(-1)
      expect(subject.pop).to eql(-1)
    end

    context 'longer tests' do
      subject { TspKit::Utils::PriorityQueue.new(100) }

      it "can push twenty ids and priorities, then get them back in priority order" do
        inputs = [
          [0, 8.9, 14], [1, 7.9, 9], [2, 20.0, 4], [3, 14.0, 16], [4, 59.3, 18], [5, 97.5, 0], [6,
          42.9, 7], [7, 3.1, 2], [8, 82.7, 17], [9, 98.0, 16], [10, 60.4, 0], [11, 32.7, 11], [12,
          99.4, 19], [13, 31.4, 17], [14, 33.0, 2], [15, 23.8, 14], [16, 99.7, 19], [17, 8.0, 13],
          [18, 34.4, 2], [19, 35.7, 4],
        ]

        expected_outputs = [
          [7, 3.1, 2], [1, 7.9, 9], [17, 8.0, 13], [0, 8.9, 14], [3, 14.0, 16], [2, 20.0, 4], [15,
          23.8, 14], [13, 31.4, 17], [11, 32.7, 11], [14, 33.0, 2], [18, 34.4, 2], [19, 35.7, 4],
          [6, 42.9, 7], [4, 59.3, 18], [10, 60.4, 0], [8, 82.7, 17], [5, 97.5, 0], [9, 98.0, 16],
          [12, 99.4, 19], [16, 99.7, 19]
        ]

        inputs.each { |args| subject.push(*args) }

        expected_outputs.each do |expected_id, expected_priority, expected_payload|
          expect(subject.peek_payload).to eql(expected_payload)
          expect(subject.peek_priority).to eql(expected_priority)
          expect(subject.peek).to eql(expected_id)
          expect(subject.pop).to eql(expected_id)
        end
      end

      it "can push forty ids and priorities with repeats, split, then get them back in priority order" do
        inputs01 = [
          [0, 8.9, 14], [1, 7.9, 9], [2, 20.0, 4], [3, 14.0, 16], [4, 77.3, 11], [6, 45.0, 6],
          [7, 3.1, 2], [8, 82.7, 17], [9, 99.0, 19], [10, 62.4, 1], [11, 32.7, 11], [12, 99.4, 19],
          [13, 31.4, 17], [14, 33.0, 2], [15, 23.8, 14], [16, 99.9, 19], [17, 8.0, 13], [18, 34.4, 2]
        ]

        expected_outputs01 = [
          [7, 3.1, 2], [1, 7.9, 9], [17, 8.0, 13], [0, 8.9, 14], [3, 14.0, 16], [2, 20.0, 4], [15, 23.8, 14],
          [13, 31.4, 17], [11, 32.7, 11], [14, 33.0, 2], [18, 34.4, 2]
        ]

        inputs01.each { |args| subject.push(*args) }

        expected_outputs01.each do |expected_id, expected_priority, expected_payload|
          # p [expected_id, expected_priority, expected_payload]
          expect(subject.peek_payload).to eql(expected_payload)
          expect(subject.peek_priority).to eql(expected_priority)
          expect(subject.peek).to eql(expected_id)
          expect(subject.pop).to eql(expected_id)
        end

        inputs02 = [
          [1, 14.5, 9], [2, 10.0, 4], [4, 59.3, 18], [5, 97.5, 0], [6, 42.9, 7],
          [8, 82.7, 17], [9, 98.0, 16], [10, 60.4, 0], [11, 30.7, 11], [12, 99.4, 19],
          [13, 31.4, 17], [15, 23.8, 14], [16, 99.7, 19], [17, 8.0, 13], [18, 34.4, 2], [19, 35.7, 4],
        ]

        expected_outputs02 = [
          [19, 35.7, 4], [6, 42.9, 7], [4, 59.3, 18], [10, 60.4, 0], [8, 82.7, 17], [5, 97.5, 0],
          [9, 98.0, 16], [12, 99.4, 19], [16, 99.7, 19]
        ]

        inputs02.each { |args| subject.push(*args) }

        expected_outputs02.each do |expected_id, expected_priority, expected_payload|
          expect(subject.peek_payload).to eql(expected_payload)
          expect(subject.peek_priority).to eql(expected_priority)
          expect(subject.peek).to eql(expected_id)
          expect(subject.pop).to eql(expected_id)
        end
      end

      it "can push forty ids and priorities with repeats, then get them back in priority order" do
        inputs = [
          [0, 9.1, 12], [1, 7.9, 9], [2, 20.1, 3], [3, 14.0, 16], [4, 59.4, 19], [5, 97.5, 0], [6, 47.9, 9],
          [0, 8.9, 14], [1, 8.9, 6], [2, 20.0, 4], [3, 15.0, 15], [4, 59.3, 18], [5, 97.8, 1], [6, 42.9, 7],
          [7, 3.2, 1], [8, 82.7, 17], [9, 98.0, 16], [10, 60.5, 1], [11, 32.7, 11], [12, 99.6, 20],
          [7, 3.1, 2], [8, 82.8, 18], [9, 98.9, 12], [10, 60.4, 0], [11, 32.9, 12], [12, 99.4, 19],
          [13, 31.6, 19], [14, 33.0, 2], [15, 23.9, 10], [16, 99.7, 19], [17, 9.0, 10], [18, 34.4, 2], [19, 35.9, 5],
          [13, 31.4, 17], [14, 33.2, 4], [15, 23.8, 14], [16, 99.9, 20], [17, 8.0, 13], [18, 34.5, 1], [19, 35.7, 4],
        ]

        expected_outputs = [
          [7, 3.1, 2], [1, 7.9, 9], [17, 8.0, 13], [0, 8.9, 14], [3, 14.0, 16], [2, 20.0, 4], [15, 23.8, 14],
          [13, 31.4, 17], [11, 32.7, 11], [14, 33.0, 2], [18, 34.4, 2], [19, 35.7, 4], [6, 42.9, 7], [4, 59.3, 18],
          [10, 60.4, 0], [8, 82.7, 17], [5, 97.5, 0], [9, 98.0, 16], [12, 99.4, 19], [16, 99.7, 19]
        ]

        inputs.each { |args| subject.push(*args) }

        expected_outputs.each do |expected_id, expected_priority, expected_payload|
          # p [expected_id, expected_priority, expected_payload]
          expect(subject.peek_payload).to eql(expected_payload)
          expect(subject.peek_priority).to eql(expected_priority)
          expect(subject.peek).to eql(expected_id)
          expect(subject.pop).to eql(expected_id)
        end
      end
    end

    it "can re-prioritise to higher priority" do
      subject.push(7, 1.3, 4)
      subject.push(6, 1.8, 3)
      subject.push(5, 0.7, 2)
      subject.push(6, 0.3, 8)

      expect(subject.peek_payload).to eql(8)
      expect(subject.pop).to eql(6)

      expect(subject.peek_payload).to eql(2)
      expect(subject.pop).to eql(5)

      expect(subject.peek_payload).to eql(4)
      expect(subject.pop).to eql(7)

      expect(subject.peek_payload).to eql(-1)
      expect(subject.pop).to eql(-1)
    end

    it "doesn't re-prioritise to lower priority" do
      subject.push(7, 1.3, 4)
      subject.push(6, 1.8, 5)
      subject.push(5, 0.7, 2)
      subject.push(7, 2.8, 8)

      expect(subject.peek_payload).to eql(2)
      expect(subject.pop).to eql(5)

      expect(subject.peek_payload).to eql(4)
      expect(subject.pop).to eql(7)

      expect(subject.peek_payload).to eql(5)
      expect(subject.pop).to eql(6)

      expect(subject.peek_payload).to eql(-1)
      expect(subject.pop).to eql(-1)
    end

    it "handles mixed push and pop" do
      subject.push(7, 1.3, 0)
      subject.push(6, 1.8, 1)
      subject.push(5, 2.9, 2)

      expect(subject.peek_payload).to eql(0)
      expect(subject.pop).to eql(7)

      expect(subject.peek_payload).to eql(1)
      expect(subject.pop).to eql(6)

      subject.push(9, 3.3, 9)
      subject.push(8, 0.5, 8)

      expect(subject.peek_payload).to eql(8)
      expect(subject.pop).to eql(8)

      expect(subject.peek_payload).to eql(2)
      expect(subject.pop).to eql(5)

      expect(subject.peek_payload).to eql(9)
      expect(subject.pop).to eql(9)

      expect(subject.peek_payload).to eql(-1)
      expect(subject.pop).to eql(-1)
    end

    it "ignores new priorities for completed items" do
      subject.push(7, 1.3, 5)
      subject.push(6, 1.8, 4)
      subject.push(5, 2.9, 2)

      expect(subject.peek_payload).to eql(5)
      expect(subject.pop).to eql(7)

      expect(subject.peek_payload).to eql(4)
      expect(subject.pop).to eql(6)

      subject.push(7, 0.3, 9)
      subject.push(9, 3.3, 1)
      subject.push(8, 0.5, 12)
      subject.push(6, 0.8, 9)

      expect(subject.peek_payload).to eql(12)
      expect(subject.pop).to eql(8)

      expect(subject.peek_payload).to eql(2)
      expect(subject.pop).to eql(5)

      expect(subject.peek_payload).to eql(1)
      expect(subject.pop).to eql(9)

      expect(subject.peek_payload).to eql(-1)
      expect(subject.pop).to eql(-1)
    end

    [124781, 12414325, 141251, 431251, 431252, 431253, 431254, 431255].each do |seed|
      it "nothing bad happens if we try to over-fill the queue (seed #{seed})" do
        srand(seed)
        collected = []
        10.times do |i|
          subject.push(i, rand(), 1 )
        end

        5.times do |i|
          collected << subject.pop
        end

        10.times do |i|
          subject.push(i, rand(), 2 )
        end

        5.times do |i|
          collected << subject.pop
        end

        expect(collected).to match_array [*0..9]

        10.times { |i| subject.push(i, rand(), 3 ) }
        expect(subject.pop).to eql(-1)
      end
    end

    [124781, 12414325, 141251, 431251, 431252, 431253, 431254, 431255].each do |seed|
      it "nothing bad happens if we try to over-fill a large queue (seed #{seed})" do
        srand(seed)
        pq = TspKit::Utils::PriorityQueue.new(200)
        collected = []

        [*0..99].each do |i|
          pq.push(i, rand(), 1 )
        end

        40.times do |i|
          collected << pq.pop
        end

        [*50..149].each do |i|
          pq.push(i, rand(), 2 )
        end

        40.times do |i|
          collected << pq.pop
        end

        [*100..199].each do |i|
          pq.push(i, rand(), 3 )
        end

        40.times do |i|
          collected << pq.pop
        end

        [*100..199].each do |i|
          pq.push(i, rand(), 4 )
        end

        80.times do |i|
          collected << pq.pop
        end

        expect(collected).to match_array [*0..199]

        10.times { |i| pq.push(i, rand(), 5 ) }
        expect(subject.pop).to eql(-1)
      end
    end
  end
end
