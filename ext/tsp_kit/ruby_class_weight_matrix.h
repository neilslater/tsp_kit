// ext/tsp_kit/ruby_class_weight_matrix.h

#ifndef RUBY_CLASS_WEIGHT_MATRIX_H
#define RUBY_CLASS_WEIGHT_MATRIX_H

#include <ruby.h>
#include "narray.h"
#include "struct_weight_matrix.h"
#include "shared_vars.h"

void init_weight_matrix_class( );

VALUE weight_matrix_as_ruby_class( WeightMatrix *weight_matrix , VALUE klass );

VALUE weight_matrix_alloc( VALUE klass );

WeightMatrix *get_weight_matrix_struct( VALUE obj );

void assert_value_wraps_weight_matrix( VALUE obj );

#endif
