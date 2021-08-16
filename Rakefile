# frozen_string_literal: true

require 'rspec/core/rake_task'
require 'rake/extensiontask'

# Insert project local lib folder into library search path
$LOAD_PATH << File.join(File.dirname(__FILE__), 'lib')

desc 'Unit tests'
RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = 'spec/*_spec.rb'
  t.verbose = true
end

Rake::ExtensionTask.new do |ext|
  ext.name = 'tsp_kit'
  ext.source_pattern = '*.{c,h}'
  ext.ext_dir = 'ext/tsp_kit'
  ext.lib_dir = 'lib/tsp_kit'
end

task default: %i[compile test]
