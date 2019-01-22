require 'helpers'

describe TspKit do
  describe "random number generator" do
    it "does not use the default mt.c seed when loaded" do
      # Run in a separate process in order to get library loaded with initial state
      script_output = `ruby -Ilib -e "require 'tsp_kit'; puts TspKit.rand"`
      got_num = script_output.chomp.to_f
      expect( got_num ).to_not be_within(1e-8).of 0.3147237002849579
    end

    it "generates numbers consistently when seeded" do
      inputs = [
        [     0, [0.048813, 0.092844, 0.215189, 0.344266, 0.102763, 0.357945, 0.044883, 0.347252, 0.923654] ],
        [   830, [0.958232, 0.129571, 0.470219, 0.028181, 0.131809, 0.675169, 0.473927, 0.302314, 0.488483] ],
        [  7685, [0.608384, 0.474372, 0.841602, 0.002379, 0.250629, 0.524337, 0.009315, 0.591537, 0.874626] ],
        [  7684, [0.639186, 0.009604, 0.583700, 0.239459, 0.475690, 0.691767, 0.598912, 0.521072, 0.958775] ],
      ]

      inputs.each do |seed, expected_results|
        TspKit.srand( seed )
        got_results = expected_results.map { |e| TspKit.rand }
        got_results.zip( expected_results ).each do |got_val, expected_val|
          expect( got_val ).to be_within(1e-6).of expected_val
        end
      end
    end

    it "can accept seeds which are arrays of integers" do
      inputs = [
        [ [0,1], [0.612994, 0.851311, 0.917829, 0.434721, 0.516676, 0.522821, 0.111721, 0.376093, 0.305647] ],
        [ [1,0], [0.634364, 0.069204, 0.347434, 0.302265, 0.263775, 0.563107, 0.755069, 0.617918, 0.995435] ],
        [ [7685,7231,2321,2322,75], [0.024745, 0.744554, 0.867087, 0.027255, 0.500870, 0.080460, 0.808293, 0.595685, 0.960167] ],
        [ [2013], [0.925177, 0.328296, 0.551907, 0.356516, 0.901791, 0.304950, 0.400617, 0.839359, 0.255680] ],
      ]

      inputs.each do |seed, expected_results|
        TspKit.srand_array( seed )
        got_results = expected_results.map { |e| TspKit.rand }
        got_results.zip( expected_results ).each do |got_val, expected_val|
          expect( got_val ).to be_within(1e-6).of expected_val
        end
      end
    end
  end

  describe ".randn" do
    it "generates sequences from normal distribution" do
      inputs = [
        [     0, [  2.051102,  1.353689, -0.978547,  1.454275, -1.338440,  1.661093, -1.429422,  2.040621,  0.284341] ],
        [   830, [  0.200539,  0.212403,  1.209253,  0.216380, -0.912048, -1.794705, -0.394491,  1.156624] ],
        [  7685, [ -0.984045,  0.159842,  0.587212,  0.008777, -1.644188, -0.253400, -2.566122, -1.663499,  0.493266] ],
        [  7684, [ 0.944384,  0.057058,  0.068674,  1.035382, -0.436135, -1.138316, -1.003699, -0.133672] ],
      ]

      inputs.each do |seed, expected_results|
        TspKit.srand( seed )
        got_results = expected_results.map { |e| TspKit.randn }
        got_results.zip( expected_results ).each do |got_val, expected_val|
          expect( got_val ).to be_within(1e-6).of expected_val
        end
      end
    end
  end

  describe ".shuffle_narray" do
    it "shuffles numbers consistently when seeded" do
      inputs = [
        [     0, [5, 7, 0, 6, 8, 3, 1, 9, 4, 2] ],
        [   830, [1, 5, 6, 0, 7, 9, 3, 2, 8, 4] ],
        [  7685, [4, 1, 2, 5, 0, 9, 3, 6, 8, 7] ],
        [  7684, [7, 3, 9, 0, 2, 1, 8, 5, 6, 4] ],
      ]

      inputs.each do |seed, expected_results|
        TspKit.srand( seed )
        narray = NArray[*0..9]
        TspKit.shuffle_narray(narray)
        expect(narray.to_a).to eql expected_results
      end
    end

    it "can shuffle a large array" do
      TspKit.srand( 1234621 )
      narray = NArray.int(200000).indgen!
      TspKit.shuffle_narray(narray)
      expect(narray[500..519].to_a).to eql [37544, 132336, 13460, 78671, 46310, 44317, 126946,
                                            113401, 45819, 56437, 69486, 18315, 20744, 172846,
                                            54965, 82657, 70511, 5599, 44508, 27858]
    end
  end
end
