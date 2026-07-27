# frozen_string_literal: true

unless ENV['TSP_KIT_DISABLE_SIMPLECOV']
  require 'simplecov'

  SimpleCov.start do
    enable_coverage :branch
    add_filter '/spec/'
    minimum_coverage line: 95, branch: 95
  end
end
