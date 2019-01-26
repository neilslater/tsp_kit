// ext/tsp_kit/ruby_class_greedy_solver.h

#ifndef RUBY_CLASS_GREEDY_SOLVER_H
#define RUBY_CLASS_GREEDY_SOLVER_H

#include <ruby.h>
#include "narray.h"
#include "struct_greedy_solver.h"
#include "shared_vars.h"

void init_greedy_solver_class( );

VALUE greedy_solver_as_ruby_class( GreedySolver *greedy_solver , VALUE klass );

VALUE greedy_solver_alloc( VALUE klass );

GreedySolver *get_greedy_solver_struct( VALUE obj );

void assert_value_wraps_greedy_solver( VALUE obj );

#endif
