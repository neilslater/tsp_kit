// ext/tsp_kit/struct_priority_queue.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for PriorityQueue and declarations for its memory management
//

#ifndef STRUCT_PRIORITY_QUEUE_H
#define STRUCT_PRIORITY_QUEUE_H

#define DBL_MAX 1.7976931348623156E+308

#include <ruby.h>
#include "narray.h"

typedef struct _priority_queue_raw {
  int pq_size;
  double *priorities;
  int *payloads;
  int *heap_leftmost_child;
  int *heap_parent;
  int *heap_left_sibling;
  int *heap_right_sibling;
  int *root_candidates;
  int *first_pass_roots;
  int heap_root;
  } PriorityQueue;

PriorityQueue *priority_queue__create();

void priority_queue__init( PriorityQueue *priority_queue, int pq_size );

void priority_queue__destroy( PriorityQueue *priority_queue );

void priority_queue__gc_mark( PriorityQueue *priority_queue );

void priority_queue__deep_copy( PriorityQueue *priority_queue_copy, PriorityQueue *priority_queue_orig );

PriorityQueue * priority_queue__clone( PriorityQueue *priority_queue_orig );

#endif
