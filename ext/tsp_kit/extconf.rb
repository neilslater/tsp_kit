# frozen_string_literal: true

# ext/tsp_kit/extconf.rb
require 'mkmf'
require 'narray'

# Following code stolen shamelessly from fftw3 gem:
narray_dir = begin
  File.dirname(Gem.find_files('narray.h').first)
rescue StandardError
  $sitearchdir
end
dir_config('narray', narray_dir, narray_dir)

unless have_header('narray.h') && have_header('narray_config.h')
  print <<-EOS
   ** configure error **
   Header narray.h or narray_config.h is not found. If you have these files in
   /narraydir/include, try the following:

   % ruby extconf.rb --with-narray-include=/narraydir/include

  EOS
  exit(-1)
end

# This also stolen from fftw3 gem (and not confirmed for Windows platforms - please let me know if it works!)
if /cygwin|mingw/ =~ RUBY_PLATFORM
  have_library('narray') || raise('ERROR: narray library is not found')
end

makefile_config = RbConfig::MAKEFILE_CONFIG
makefile_config['CC'] = ENV['CC'] if ENV['CC']
makefile_config['CFLAGS'] << ' -std=c99'

with_cflags(makefile_config['CFLAGS']) { create_makefile('tsp_kit/tsp_kit') }
