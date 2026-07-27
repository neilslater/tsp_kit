# frozen_string_literal: true

# ext/tsp_kit/extconf.rb
require 'mkmf'
require 'numo/narray/alt'

$LOAD_PATH.each do |load_path|
  next unless File.exist?(File.join(load_path, 'numo/numo/narray.h'))

  $INCFLAGS = "-I#{File.join(load_path, 'numo')} #{$INCFLAGS}"
  break
end

abort 'numo/narray.h not found' unless have_header('numo/narray.h')

if RUBY_PLATFORM.include?('darwin') &&
   try_link('int main(void) { return 0; }', '-Wl,-undefined,dynamic_lookup')
  $LDFLAGS << ' -Wl,-undefined,dynamic_lookup'
end

makefile_config = RbConfig::MAKEFILE_CONFIG
makefile_config['CC'] = ENV['CC'] if ENV['CC']
makefile_config['CFLAGS'] << ' -std=c99'

with_cflags(makefile_config['CFLAGS']) { create_makefile('tsp_kit/tsp_kit') }
