// ext/tsp_kit/ruby_class_one_tree.h

#ifndef RUBY_CLASS_ONE_TREE_H
#define RUBY_CLASS_ONE_TREE_H

#include <ruby.h>
#include "narray.h"
#include "struct_one_tree.h"
#include "shared_vars.h"

void init_one_tree_class( );

VALUE one_tree_as_ruby_class( OneTree *one_tree , VALUE klass );

VALUE one_tree_alloc( VALUE klass );

OneTree *get_one_tree_struct( VALUE obj );

void assert_value_wraps_one_tree( VALUE obj );

#endif
