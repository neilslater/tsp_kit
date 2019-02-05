// ext/tsp_kit/struct_priority_queue.c

#include "struct_priority_queue.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for PriorityQueue memory management
//

PriorityQueue *priority_queue__create() {
  PriorityQueue *pq;
  pq = xmalloc( sizeof(PriorityQueue) );
  pq->pq_size = 0;

  pq->priorities = NULL;
  pq->payloads = NULL;
  pq->heap_leftmost_child = NULL;
  pq->heap_parent = NULL;
  pq->heap_left_sibling = NULL;
  pq->heap_right_sibling = NULL;

  // These are scratch space
  pq->root_candidates = NULL;
  pq->first_pass_roots = NULL;

  pq->heap_root = -1; // Empty
  return pq;
}

void priority_queue__init( PriorityQueue *pq, int pq_size ) {
  int i;

  pq->pq_size = pq_size;
  pq->heap_root = -1; // Empty

  pq->priorities = ALLOC_N( double, pq_size );
  pq->payloads = ALLOC_N( int, pq_size );
  pq->heap_leftmost_child = ALLOC_N( int, pq_size );
  pq->heap_parent = ALLOC_N( int, pq_size );
  pq->heap_left_sibling = ALLOC_N( int, pq_size );
  pq->heap_right_sibling = ALLOC_N( int, pq_size );

  pq->root_candidates = ALLOC_N( int, pq_size );
  pq->first_pass_roots = ALLOC_N( int, pq_size );

  for( i = 0; i < pq_size; i++ ) {
    pq->priorities[i] = DBL_MAX;
    pq->payloads[i] = -1;
    pq->heap_leftmost_child[i] = -1;
    pq->heap_parent[i] = -1;
    pq->heap_left_sibling[i] = -1;
    pq->heap_right_sibling[i] = -1;
  }

  return;
}

void priority_queue__destroy( PriorityQueue *pq ) {
  xfree( pq->priorities );
  xfree( pq->payloads );
  xfree( pq->heap_leftmost_child );
  xfree( pq->heap_parent );
  xfree( pq->heap_left_sibling );
  xfree( pq->heap_right_sibling );
  xfree( pq->root_candidates );
  xfree( pq->first_pass_roots );
  xfree( pq );
  return;
}

void priority_queue__gc_mark( PriorityQueue *pq ) {
  return;
}

void priority_queue__deep_copy( PriorityQueue *pq_copy, PriorityQueue *pq_orig ) {
  int pq_size = pq_orig->pq_size;

  pq_copy->pq_size = pq_orig->pq_size;
  pq_copy->heap_root = pq_orig->heap_root;

  pq_copy->priorities = ALLOC_N( double, pq_size );
  pq_copy->payloads = ALLOC_N( int, pq_size );
  pq_copy->heap_leftmost_child = ALLOC_N( int, pq_size );
  pq_copy->heap_parent = ALLOC_N( int, pq_size );
  pq_copy->heap_left_sibling = ALLOC_N( int, pq_size );
  pq_copy->heap_right_sibling = ALLOC_N( int, pq_size );

  pq_copy->root_candidates = ALLOC_N( int, pq_size );
  pq_copy->first_pass_roots = ALLOC_N( int, pq_size );

  memcpy( pq_copy->priorities, pq_orig->priorities, ( pq_size ) * sizeof(double) );
  memcpy( pq_copy->payloads, pq_orig->payloads, ( pq_size ) * sizeof(int) );
  memcpy( pq_copy->heap_leftmost_child, pq_orig->heap_leftmost_child, ( pq_size ) * sizeof(int) );
  memcpy( pq_copy->heap_parent, pq_orig->heap_parent, ( pq_size ) * sizeof(int) );
  memcpy( pq_copy->heap_left_sibling, pq_orig->heap_left_sibling, ( pq_size ) * sizeof(int) );
  memcpy( pq_copy->heap_right_sibling, pq_orig->heap_right_sibling, ( pq_size ) * sizeof(int) );

  return;
}

PriorityQueue * priority_queue__clone( PriorityQueue *pq_orig ) {
  PriorityQueue * pq_copy = priority_queue__create();
  priority_queue__deep_copy( pq_copy, pq_orig );
  return pq_copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int priority_queue__find_min_id( PriorityQueue *pq ) {
  return pq->heap_root;
}

double priority_queue__find_min_priority( PriorityQueue *pq ) {
  if (pq->heap_root < 0) {
    return -DBL_MAX;
  }
  return pq->priorities[pq->heap_root];
}

double priority_queue__find_min_payload( PriorityQueue *pq ) {
  if (pq->heap_root < 0) {
    return -1;
  }
  return pq->payloads[pq->heap_root];
}

// Merge returns the new, combined root
int priority_queue__merge( PriorityQueue *pq, int a_id, int b_id ) {
  int current_leftmost_child;

  if (a_id < 0) {
    return b_id;
  }
  if (b_id < 0) {
    return a_id;
  }

  if (pq->priorities[a_id] < pq->priorities[b_id]) {
    // New root on A
    current_leftmost_child = pq->heap_leftmost_child[a_id];
    if (current_leftmost_child > -1) {
      pq->heap_left_sibling[current_leftmost_child] = b_id;
      pq->heap_right_sibling[b_id] = current_leftmost_child;
    }
    pq->heap_leftmost_child[a_id] = b_id;
    pq->heap_parent[b_id] = a_id;
    return a_id;
  }

  // New root on B
  current_leftmost_child = pq->heap_leftmost_child[b_id];
  if (current_leftmost_child > -1) {
    pq->heap_left_sibling[current_leftmost_child] = a_id;
    pq->heap_right_sibling[a_id] = current_leftmost_child;
  }
  pq->heap_leftmost_child[b_id] = a_id;
  pq->heap_parent[a_id] = b_id;
  return b_id;
}

int priority_queue__delete_min( PriorityQueue *pq ) {
  int old_min, sibling_id, n_root_candidates, n_first_pass_roots, next_id;

  if ( pq->heap_root < 0 ) {
    return -1;
  }

  old_min = pq->heap_root;

  pq->priorities[old_min] = -DBL_MAX;
  pq->payloads[old_min] = -1;

  sibling_id = pq->heap_leftmost_child[old_min];
  pq->heap_leftmost_child[old_min] = -1;
  pq->heap_right_sibling[old_min] = -1;
  pq->heap_left_sibling[old_min] = -1;
  pq->heap_parent[old_min] = -1;

  pq->heap_root = -1;
  if (sibling_id < 0) {
    return old_min; // Empty
  }

  n_root_candidates = 0;
  while( sibling_id > -1 ) {
    pq->root_candidates[n_root_candidates] = sibling_id;
    n_root_candidates++;
    sibling_id = pq->heap_right_sibling[sibling_id];
  }
  pq->root_candidates[n_root_candidates] = -1;

  // Disconnect new roots from siblings and old parent
  for(int i = 0; i < n_root_candidates; i++) {
    sibling_id = pq->root_candidates[i];
    pq->heap_right_sibling[sibling_id] = -1;
    pq->heap_left_sibling[sibling_id] = -1;
    pq->heap_parent[sibling_id] = -1;
  }

  n_first_pass_roots = (n_root_candidates+1)/2;
  for(int i = 0; i < n_first_pass_roots; i++) {
    pq->first_pass_roots[i] = priority_queue__merge( pq, pq->root_candidates[i*2], pq->root_candidates[i*2+1] );
  }

  pq->heap_root = pq->first_pass_roots[n_first_pass_roots - 1];
  for(int i = n_first_pass_roots - 2; i > -1; i--) {
    pq->heap_root = priority_queue__merge( pq, pq->heap_root, pq->first_pass_roots[i] );
  }

  return old_min;
}

// This assumes that id is not already in the heap (or already used), and the new_priority is valid
void priority_queue__insert_key( PriorityQueue *pq, int id, double new_priority, int new_payload ) {
  pq->priorities[id] = new_priority;
  pq->payloads[id] = new_payload;
  pq->heap_root = priority_queue__merge( pq, pq->heap_root, id );
  return;
}

// This assumes that id is in the heap, and the new_priority is really a decrease
void priority_queue__decrease_key( PriorityQueue *pq, int id, double new_priority, int new_payload ) {
  int parent_id, right_sibling_id, left_sibling_id;

  pq->priorities[id] = new_priority;
  pq->payloads[id] = new_payload;
  parent_id = pq->heap_parent[id];
  if (id == pq->heap_root || parent_id < 0) {
    return;
  }
  if (new_priority > pq->priorities[parent_id]) {
    return;
  }

  // Detach from siblings
  right_sibling_id = pq->heap_right_sibling[id];
  left_sibling_id = pq->heap_left_sibling[id];
  if (right_sibling_id > -1) {
    pq->heap_left_sibling[ right_sibling_id ] = left_sibling_id;
  }
  if (left_sibling_id > -1) {
    pq->heap_right_sibling[ left_sibling_id ] = right_sibling_id;
  }

  // Detach from parent
  if (pq->heap_leftmost_child[parent_id] == id) {
    pq->heap_leftmost_child[parent_id] = right_sibling_id;
  }

  // Clean up to be separate root, then merge back in
  pq->heap_right_sibling[id] = -1;
  pq->heap_left_sibling[id] = -1;
  pq->heap_parent[id] = -1;
  pq->heap_root = priority_queue__merge(pq, pq->heap_root, id);
  return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Public API

void priority_queue__push( PriorityQueue *pq, int id, double priority, int payload ) {
  if ( (pq->priorities[id] < priority) ) {
    return;
  }

  if (pq->priorities[id] > 1.0e300) {
    priority_queue__insert_key(pq, id, priority, payload);
  } else {
    priority_queue__decrease_key(pq, id, priority, payload);
  }

  return;
}

int priority_queue__pop( PriorityQueue *pq ) {
  return priority_queue__delete_min(pq);
}

int priority_queue__peek( PriorityQueue *pq ) {
  return priority_queue__find_min_id(pq);
}

double priority_queue__peek_priority( PriorityQueue *pq ) {
  return priority_queue__find_min_priority(pq);
}

int priority_queue__peek_payload( PriorityQueue *pq ) {
  return priority_queue__find_min_payload(pq);
}
