// ext/tsp_kit/ruby_class_priority_queue.h

#ifndef RUBY_CLASS_PRIORITY_QUEUE_H
#define RUBY_CLASS_PRIORITY_QUEUE_H

#include <ruby.h>
#include "narray.h"
#include "struct_priority_queue.h"
#include "shared_vars.h"

void init_priority_queue_class( );

VALUE priority_queue_as_ruby_class( PriorityQueue *priority_queue , VALUE klass );

VALUE priority_queue_alloc( VALUE klass );

PriorityQueue *get_priority_queue_struct( VALUE obj );

void assert_value_wraps_priority_queue( VALUE obj );

#endif
