// ext/tsp_kit/ruby_class_distance_rank.h

#ifndef RUBY_CLASS_DISTANCE_RANK_H
#define RUBY_CLASS_DISTANCE_RANK_H

#include <ruby.h>
#include "narray.h"
#include "struct_distance_rank.h"
#include "shared_vars.h"

void init_distance_rank_class( );

VALUE distance_rank_as_ruby_class( DistanceRank *distance_rank , VALUE klass );

VALUE distance_rank_alloc(VALUE klass);

DistanceRank *get_distance_rank_struct( VALUE obj );

void assert_value_wraps_distance_rank( VALUE obj );

#endif
