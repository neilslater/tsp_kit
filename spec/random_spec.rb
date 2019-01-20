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
end
