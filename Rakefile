# frozen_string_literal: true

require 'fileutils'
require 'open3'
require 'rspec/core/rake_task'
require 'rake/extensiontask'
require 'rbconfig'
require 'rubygems/package_task'
require 'shellwords'
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
Rake::Task[:coverage].enhance([:compile])

YARD::Rake::YardocTask.new(:yard) do |t|
  t.files = ['lib/**/*.rb']
  t.options = ['--fail-on-warning', '--no-private']
end

rebuild_and_test_native = lambda do |mode, test: true|
  tasks = %w[clobber compile]
  tasks << 'test' if test

  sh(
    { 'TSP_KIT_NATIVE_MODE' => mode },
    RbConfig.ruby,
    '-S',
    'bundle',
    'exec',
    'rake',
    *tasks
  )
end

gcc_compiler = lambda do |task_name|
  cc = RbConfig::CONFIG.fetch('CC')
  compiler_version = Open3.capture2e(*Shellwords.split(cc), '--version').first

  unless compiler_version.match?(/gcc/i) && !compiler_version.match?(/clang/i)
    abort "#{task_name} requires a GCC Ruby build (current compiler: #{cc})"
  end

  cc
end

# The native tasks keep their platform checks and build orchestration together.
# rubocop:disable Metrics/BlockLength
namespace :c do
  desc 'Compile the C extension with strict warnings'
  task :lint do
    rebuild_and_test_native.call('lint', test: false)
  end

  desc 'Measure C coverage using the full Ruby spec suite'
  task :coverage do
    gcc_compiler.call('c:coverage')
    abort 'c:coverage requires gcovr on PATH' unless system('gcovr', '--version', out: File::NULL)

    rebuild_and_test_native.call('coverage')

    FileUtils.mkdir_p('coverage/c')
    sh(
      'gcovr',
      '--root', '.',
      '--filter', 'ext/tsp_kit/',
      '--html-details', 'coverage/c/index.html',
      '--xml', 'coverage/c/cobertura.xml',
      '--txt', 'coverage/c/summary.txt',
      '--print-summary'
    )
  end

  desc 'Run the Ruby specs with ASan and UBSan'
  task :sanitize do
    abort 'c:sanitize requires Linux' unless RUBY_PLATFORM.match?(/linux/)

    cc = gcc_compiler.call('c:sanitize')
    libasan = Open3.capture2e(*Shellwords.split(cc), '-print-file-name=libasan.so').first.strip
    abort 'c:sanitize could not locate the GCC ASan runtime' if libasan.empty? || libasan == 'libasan.so'

    rebuild_and_test_native.call('sanitize', test: false)

    sh(
      {
        'ASAN_OPTIONS' => 'detect_leaks=0',
        'TSP_KIT_DISABLE_SIMPLECOV' => '1',
        'LD_PRELOAD' => libasan
      },
      RbConfig.ruby,
      '-S',
      'bundle',
      'exec',
      'rake',
      'test'
    )
  end
end
# rubocop:enable Metrics/BlockLength

Rake::ExtensionTask.new('tsp_kit', gemspec) do |ext|
  ext.source_pattern = '*.{c,h}'
  ext.ext_dir = 'ext/tsp_kit'
  ext.lib_dir = 'lib/tsp_kit'
end

task default: %i[compile test]
