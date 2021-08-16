# frozen_string_literal: true

module TspKit
  class Solver
    def initialize(opts = {})
      set_opts(opts)
    end

    def run
      raise NotImplementedError
    end

    def self.default_opts
      { example: 0 }
    end

    private

    def set_opts(opts)
      @opts = self.class.default_opts.merge(opts)
    end
  end
end
