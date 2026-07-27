# TspKit

TspKit is a set of utility classes for approximately solving Travelling Salesman Problem data sets, and
related problems.

The main solver is inspired by "An Effective Implementation of the
Lin-Kernighan Traveling Salesman Heuristic" by Keld Helsgaun, which can be found at
http://akira.ruc.dk/~keld/research/LKH/LKH-2.0/DOC/LKH_REPORT.pdf

## Installation

TspKit supports Ruby 3.3, 3.4, and 4.0, and uses
[`numo-narray-alt`](https://rubygems.org/gems/numo-narray-alt) 0.10.5.
A C compiler and the normal Ruby native-extension build tools are required.

Install the gem:

```sh
gem install tsp_kit
```

For development, install dependencies and run a clean build:

```sh
bundle install
bundle exec rake clobber
bundle exec rake
```

The final command compiles the C extension and runs the complete spec suite.
Run `bundle exec rake gem` to build `pkg/tsp_kit-0.0.1.gem`.

## Array API

Public numerical values use explicit Numo types:

- coordinates, weights, penalties, and distances use `Numo::DFloat`;
- node identifiers, ranks, and solution indexes use `Numo::Int32`;
- two-dimensional arrays use conventional row-major shapes. For example,
  Euclidean coordinates have shape `[num_nodes, num_dims]`.

Legacy top-level `NArray` values and legacy Marshal files are not supported.

## Contributing

1. Fork it ( https://github.com/slobo777/tsp_kit/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
