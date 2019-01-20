// ext/tsp_kit/struct_priority_queue.c

#include "struct_priority_queue.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for PriorityQueue memory management
//

PriorityQueue *priority_queue__create() {
  PriorityQueue *priority_queue;
  priority_queue = xmalloc( sizeof(PriorityQueue) );
  priority_queue->pq_size = 0;
  priority_queue->priorities = NULL;
  priority_queue->payloads = NULL;
  priority_queue->heap_leftmost_child = NULL;
  priority_queue->heap_parent = NULL;
  priority_queue->heap_left_sibling = NULL;
  priority_queue->heap_right_sibling = NULL;
  priority_queue->root_candidates = NULL;
  priority_queue->first_pass_roots = NULL;
  priority_queue->heap_root = 0;
  return priority_queue;
}

void priority_queue__init( PriorityQueue *priority_queue, int pq_size ) {
  int i;

  priority_queue->pq_size = pq_size;

  priority_queue->heap_root = -1;

  priority_queue->priorities = ALLOC_N( double, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->priorities[i] = DBL_MAX;
  }

  priority_queue->payloads = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->payloads[i] = -1;
  }

  priority_queue->heap_leftmost_child = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->heap_leftmost_child[i] = -1;
  }

  priority_queue->heap_parent = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->heap_parent[i] = -1;
  }

  priority_queue->heap_left_sibling = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->heap_left_sibling[i] = -1;
  }

  priority_queue->heap_right_sibling = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->heap_right_sibling[i] = -1;
  }

  priority_queue->root_candidates = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->root_candidates[i] = -1;
  }

  priority_queue->first_pass_roots = ALLOC_N( int, pq_size );
  for( i = 0; i < pq_size; i++ ) {
    priority_queue->first_pass_roots[i] = -1;
  }

  return;
}

void priority_queue__destroy( PriorityQueue *priority_queue ) {
  xfree( priority_queue->priorities );
  xfree( priority_queue->payloads );
  xfree( priority_queue->heap_leftmost_child );
  xfree( priority_queue->heap_parent );
  xfree( priority_queue->heap_left_sibling );
  xfree( priority_queue->heap_right_sibling );
  xfree( priority_queue->root_candidates );
  xfree( priority_queue->first_pass_roots );
  xfree( priority_queue );
  return;
}

void priority_queue__gc_mark( PriorityQueue *priority_queue ) {
  return;
}

void priority_queue__deep_copy( PriorityQueue *priority_queue_copy, PriorityQueue *priority_queue_orig ) {
  int pq_size = priority_queue_orig->pq_size;

  priority_queue_copy->pq_size = pq_size;
  priority_queue_copy->heap_root = priority_queue_orig->heap_root;

  priority_queue_copy->priorities = ALLOC_N( double, pq_size );
  memcpy( priority_queue_copy->priorities, priority_queue_orig->priorities, ( pq_size ) * sizeof(double) );

  priority_queue_copy->payloads = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->payloads, priority_queue_orig->payloads, ( pq_size ) * sizeof(int) );

  priority_queue_copy->heap_leftmost_child = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->heap_leftmost_child, priority_queue_orig->heap_leftmost_child, ( pq_size ) * sizeof(int) );

  priority_queue_copy->heap_parent = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->heap_parent, priority_queue_orig->heap_parent, ( pq_size ) * sizeof(int) );

  priority_queue_copy->heap_left_sibling = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->heap_left_sibling, priority_queue_orig->heap_left_sibling, ( pq_size ) * sizeof(int) );

  priority_queue_copy->heap_right_sibling = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->heap_right_sibling, priority_queue_orig->heap_right_sibling, ( pq_size ) * sizeof(int) );

  priority_queue_copy->root_candidates = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->root_candidates, priority_queue_orig->root_candidates, ( pq_size ) * sizeof(int) );

  priority_queue_copy->first_pass_roots = ALLOC_N( int, pq_size );
  memcpy( priority_queue_copy->first_pass_roots, priority_queue_orig->first_pass_roots, ( pq_size ) * sizeof(int) );

  return;
}

PriorityQueue * priority_queue__clone( PriorityQueue *priority_queue_orig ) {
  PriorityQueue * priority_queue_copy = priority_queue__create();
  priority_queue__deep_copy( priority_queue_copy, priority_queue_orig );
  return priority_queue_copy;
}
