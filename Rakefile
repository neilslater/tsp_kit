# frozen_string_literal: true

require 'rspec/core/rake_task'
require 'rake/extensiontask'
require 'rubygems/package_task'

gemspec = Gem::Specification.load(File.expand_path('tsp_kit.gemspec', __dir__))
Gem::PackageTask.new(gemspec).define

# Insert project local lib folder into library search path
$LOAD_PATH << File.join(File.dirname(__FILE__), 'lib')

desc 'Unit tests'
RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = 'spec/*_spec.rb'
  t.verbose = true
end

Rake::ExtensionTask.new('tsp_kit', gemspec) do |ext|
  ext.source_pattern = '*.{c,h}'
  ext.ext_dir = 'ext/tsp_kit'
  ext.lib_dir = 'lib/tsp_kit'
end

task default: %i[compile test]
