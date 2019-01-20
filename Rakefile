require "rspec/core/rake_task"
require 'rake/extensiontask'

# Insert project local lib folder into library search path
$: << File.join( File.dirname( __FILE__ ), 'lib' )

desc "Unit tests"
RSpec::Core::RakeTask.new(:test) do |t|
  t.pattern = "spec/*_spec.rb"
  t.verbose = true
end

Rake::ExtensionTask.new do |ext|
  ext.name = 'tsp_kit'
  ext.source_pattern = "*.{c,h}"
  ext.ext_dir = 'ext/tsp_kit'
  ext.lib_dir = 'lib/tsp_kit'
end

desc "Import CSV"
task :import_csv do
  require 'tsp_kit'
  if TspKit.ready_to_run?
    puts "CSV data already imported."
  elsif TspKit.ready_to_import?
    TspKit.import_from_csv
  else
    puts ''
    puts '**********************************************************'
    puts ' Place Kaggle file data.csv in data directory'
    puts '**********************************************************'
    puts ''
    raise "Data file missing: #{TspKit::CSV_PATH}"
  end
end

desc "Run solver and save submission"
task :solve do
  require 'tsp_kit'
  unless TspKit.ready_to_run?
    raise "Not ready, import CSV data first."
  end
  solver = TspKit::Solver.new
  solver.run
  submission_file = File.join( TspKit::DATA_PATH, 'submission.csv' )
  puts "Writing submission file: #{submission_file}"
  solver.submission.write_csv( submission_file )
end

task :default => [:compile, :import_csv, :test, :solve]
