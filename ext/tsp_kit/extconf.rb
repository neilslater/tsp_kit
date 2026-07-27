# frozen_string_literal: true

# ext/tsp_kit/extconf.rb
require 'mkmf'
require 'numo/narray/alt'
require 'rbconfig'

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

native_mode = ENV.fetch('TSP_KIT_NATIVE_MODE', 'release')
native_cflags = "#{makefile_config['CFLAGS']} -std=gnu2x"

case native_mode
when 'release'
  # Retain Ruby's configured optimisation and hardening flags.
when 'lint'
  native_cflags << ' -O0 -g'
  native_cflags << ' -Wall -Wextra -Wpedantic -Wformat=2 -Werror'

  cc = RbConfig::CONFIG.fetch('CC')
  host_os = RbConfig::CONFIG.fetch('host_os')
  if cc.match?(/clang/) || host_os.match?(/darwin/)
    # Ruby's ANYARGS declarations and inline helpers trigger these warnings.
    native_cflags << ' -Wno-strict-prototypes'
    native_cflags << ' -Wno-unused-parameter'
  end
when 'coverage'
  native_cflags << ' -O0 -g --coverage'
  $LDFLAGS << ' --coverage'
when 'sanitize'
  native_cflags << ' -O1 -g -fsanitize=address,undefined'
  native_cflags << ' -fno-omit-frame-pointer'
  $LDFLAGS << ' -fsanitize=address,undefined'
else
  abort "Unknown TSP_KIT_NATIVE_MODE: #{native_mode}"
end

with_cflags(native_cflags) { create_makefile('tsp_kit/tsp_kit') }
