// ext/tsp_kit/struct_priority_queue.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for PriorityQueue and declarations for its memory management
//

#ifndef STRUCT_PRIORITY_QUEUE_H
#define STRUCT_PRIORITY_QUEUE_H

#include <ruby.h>
#include <float.h>
#include "narray.h"
#include "utilities.h"

typedef struct _priority_queue_raw {
  int pq_size;

  double *priorities;
  int *payloads;

  int *heap_leftmost_child;
  int *heap_parent;
  int *heap_left_sibling;
  int *heap_right_sibling;

  // These are scratch space
  int *root_candidates;
  int *first_pass_roots;

  int heap_root;
  } PriorityQueue;

PriorityQueue * priority_queue__create();

void priority_queue__init( PriorityQueue *pq, int pq_size );

void priority_queue__destroy( PriorityQueue *pq );

void priority_queue__gc_mark( PriorityQueue *pq );

void priority_queue__deep_copy( PriorityQueue *pq_copy, PriorityQueue *pq_orig );

PriorityQueue * priority_queue__clone( PriorityQueue *pq_orig );

void priority_queue__push( PriorityQueue *pq, int id, double priority, int payload );

int priority_queue__pop( PriorityQueue *pq );

int priority_queue__peek( PriorityQueue *pq );

double priority_queue__peek_priority( PriorityQueue *pq );

int priority_queue__peek_payload( PriorityQueue *pq );

#endif
