# TspKit

TspKit is a set of utility classes for approximately solving Travelling Salesman Problem data sets, and
related problems.

The main solver is inspired by "An Effective Implementation of the
Lin-Kernighan Traveling Salesman Heuristic" by Keld Helsgaun, which can be found at
http://akira.ruc.dk/~keld/research/LKH/LKH-2.0/DOC/LKH_REPORT.pdf

## Installation

 * You need a Ruby interpreter, recommended version 2.1.4 or higher, with support for compiling native extensions.

 * Copy this repo locally, and make the directory with the README.md your current directory

 * Install dependencies:


    $ bundle install


 * Compile and test:


    $ bundle exec rake


This should compile the C code, and run the unit tests.


## Contributing

1. Fork it ( https://github.com/slobo777/tsp_kit/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
