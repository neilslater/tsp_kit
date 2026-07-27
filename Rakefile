# frozen_string_literal: true

require 'rspec/core/rake_task'
require 'rake/extensiontask'
require 'rubygems/package_task'
require 'yard'
require 'yard/rake/yardoc_task'

gemspec = Gem::Specification.load(File.expand_path('tsp_kit.gemspec', __dir__))
Gem::PackageTask.new(gemspec).define

# Insert project local lib folder into library search path
$LOAD_PATH << File.join(File.dirname(__FILE__), 'lib')

desc 'Unit tests'
RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = 'spec/*_spec.rb'
  t.verbose = true
end

desc 'Run specs with enforced line and branch coverage'
RSpec::Core::RakeTask.new(:coverage) do |t|
  t.pattern = 'spec/*_spec.rb'
  t.rspec_opts = ['--require', File.expand_path('spec/coverage_helper.rb', __dir__)]
end

YARD::Rake::YardocTask.new(:yard) do |t|
  t.files = ['lib/**/*.rb']
  t.options = ['--fail-on-warning', '--no-private']
end

namespace :c do
  desc 'Compile the native extension with Clang warnings treated as errors'
  task lint: :clobber do
    sh({ 'CC' => 'clang', 'warnflags' => '-Wall -Wextra -Wpedantic -Werror' },
       Gem.ruby, '-S', 'bundle', 'exec', 'rake', 'compile')
  end
end

Rake::ExtensionTask.new('tsp_kit', gemspec) do |ext|
  ext.source_pattern = '*.{c,h}'
  ext.ext_dir = 'ext/tsp_kit'
  ext.lib_dir = 'lib/tsp_kit'
end

task default: %i[compile test]
