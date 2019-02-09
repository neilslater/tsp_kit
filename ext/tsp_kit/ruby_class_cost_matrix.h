// ext/tsp_kit/ruby_class_cost_matrix.h

#ifndef RUBY_CLASS_WEIGHT_MATRIX_H
#define RUBY_CLASS_WEIGHT_MATRIX_H

#include <ruby.h>
#include "narray.h"
#include "struct_cost_matrix.h"
#include "shared_vars.h"

void init_cost_matrix_class( );

VALUE cost_matrix_as_ruby_class( CostMatrix *cost_matrix , VALUE klass );

VALUE cost_matrix_alloc( VALUE klass );

CostMatrix *get_cost_matrix_struct( VALUE obj );

void assert_value_wraps_cost_matrix( VALUE obj );

#endif
