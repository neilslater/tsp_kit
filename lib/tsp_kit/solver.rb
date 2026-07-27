# frozen_string_literal: true

module TspKit
  # Base interface for solver implementations.
  class Solver
    def initialize(opts = {})
      set_opts(opts)
    end

    def run
      raise NotImplementedError
    end

    # Default solver options.
    # @return [Hash]
    def self.default_opts
      { example: 0 }
    end

    private

    def set_opts(opts)
      @opts = self.class.default_opts.merge(opts)
    end
  end
end
