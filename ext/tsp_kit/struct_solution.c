// ext/tsp_kit/struct_solution.c

#include "struct_solution.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for Solution memory management
//

Solution *solution__create() {
  Solution *solution;
  solution = xmalloc( sizeof(Solution) );
  solution->num_nodes = 0;
  solution->ids_shape = NULL;
  solution->narr_ids = Qnil;
  solution->ids = NULL;
  solution->node_idx_shape = NULL;
  solution->narr_node_idx = Qnil;
  solution->node_idx = NULL;
  return solution;
}

void solution__init( Solution *solution, int num_nodes ) {
  int i;
  struct NARRAY *narr;
  int32_t *narr_ids_ptr;
  int32_t *narr_node_idx_ptr;

  solution->num_nodes = num_nodes;

  solution->ids_shape = ALLOC_N( int, 1 );
  solution->ids_shape[0] = num_nodes;
  solution->narr_ids = na_make_object( NA_LINT, 1, solution->ids_shape, cNArray );
  GetNArray( solution->narr_ids, narr );
  narr_ids_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_ids_ptr[i] = 0;
  }
  solution->ids = (int32_t *) narr->ptr;

  solution->node_idx_shape = ALLOC_N( int, 1 );
  solution->node_idx_shape[0] = num_nodes;
  solution->narr_node_idx = na_make_object( NA_LINT, 1, solution->node_idx_shape, cNArray );
  GetNArray( solution->narr_node_idx, narr );
  narr_node_idx_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_idx_ptr[i] = 0;
  }
  solution->node_idx = (int32_t *) narr->ptr;

  return;
}

void solution__destroy( Solution *solution ) {
  xfree( solution->ids_shape );
  xfree( solution->node_idx_shape );
  xfree( solution );
  return;
}

void solution__gc_mark( Solution *solution ) {
  rb_gc_mark( solution->narr_ids );
  rb_gc_mark( solution->narr_node_idx );
  return;
}

void solution__deep_copy( Solution *solution_copy, Solution *solution_orig ) {
  struct NARRAY *narr;

  solution_copy->num_nodes = solution_orig->num_nodes;

  solution_copy->narr_ids = na_clone( solution_orig->narr_ids );
  GetNArray( solution_copy->narr_ids, narr );
  solution_copy->ids = (int32_t *) narr->ptr;
  solution_copy->ids_shape = ALLOC_N( int, 1 );
  memcpy( solution_copy->ids_shape, solution_orig->ids_shape, 1 * sizeof(int) );

  solution_copy->narr_node_idx = na_clone( solution_orig->narr_node_idx );
  GetNArray( solution_copy->narr_node_idx, narr );
  solution_copy->node_idx = (int32_t *) narr->ptr;
  solution_copy->node_idx_shape = ALLOC_N( int, 1 );
  memcpy( solution_copy->node_idx_shape, solution_orig->node_idx_shape, 1 * sizeof(int) );

  return;
}

Solution * solution__clone( Solution *solution_orig ) {
  Solution * solution_copy = solution__create();
  solution__deep_copy( solution_copy, solution_orig );
  return solution_copy;
}
