# frozen_string_literal: true

require_relative 'lib/tsp_kit/version'

Gem::Specification.new do |spec|
  spec.name = 'tsp_kit'
  spec.version = TspKit::VERSION
  spec.authors = ['Neil Slater']
  spec.email = ['slobo777@gmail.com']

  spec.summary = 'Native Ruby tools for travelling salesman problems'
  spec.description = <<~DESCRIPTION.strip
    TspKit provides Numo::NArray-backed data structures and native algorithms
    for constructing and approximately solving travelling salesman problems.
  DESCRIPTION
  spec.homepage = 'https://github.com/neilslater/tsp_kit'
  spec.license = 'MIT'
  spec.required_ruby_version = '>= 4.0', '< 4.1'

  spec.metadata = {
    'homepage_uri' => spec.homepage,
    'source_code_uri' => "#{spec.homepage}/tree/main"
  }

  spec.files = Dir[
    'LICENSE.txt',
    'README.md',
    'lib/**/*.rb',
    'ext/**/*.{c,h,rb}'
  ].sort
  spec.require_paths = ['lib']
  spec.extensions = ['ext/tsp_kit/extconf.rb']

  spec.add_dependency 'csv', '~> 3.3'
  spec.add_dependency 'numo-narray-alt', '0.10.5'
end
