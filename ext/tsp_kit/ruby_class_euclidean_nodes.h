// ext/tsp_kit/ruby_class_euclidean_nodes.h

#ifndef RUBY_CLASS_EUCLIDEAN_NODES_H
#define RUBY_CLASS_EUCLIDEAN_NODES_H

#include <ruby.h>
#include "narray.h"
#include "struct_euclidean_nodes.h"
#include "shared_vars.h"

void init_euclidean_nodes_class( );

VALUE euclidean_nodes_as_ruby_class( EuclideanNodes *euclidean_nodes , VALUE klass );

VALUE euclidean_nodes_alloc(VALUE klass);

EuclideanNodes *get_euclidean_nodes_struct( VALUE obj );

void assert_value_wraps_euclidean_nodes( VALUE obj );

#endif
