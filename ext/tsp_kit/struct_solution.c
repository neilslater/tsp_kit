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
  int32_t *narr_ids_ptr;
  int32_t *narr_node_idx_ptr;

  solution->num_nodes = num_nodes;

  solution->ids_shape = ALLOC_N( int, 1 );
  solution->ids_shape[0] = num_nodes;
  solution->narr_ids = tsp_numo_new(numo_cInt32, 1, solution->ids_shape);
  narr_ids_ptr = (int32_t *)tsp_numo_write_pointer(solution->narr_ids);
  for( i = 0; i < num_nodes; i++ ) {
    narr_ids_ptr[i] = 0;
  }
  solution->ids = narr_ids_ptr;

  solution->node_idx_shape = ALLOC_N( int, 1 );
  solution->node_idx_shape[0] = num_nodes;
  solution->narr_node_idx = tsp_numo_new(numo_cInt32, 1, solution->node_idx_shape);
  narr_node_idx_ptr = (int32_t *)tsp_numo_write_pointer(solution->narr_node_idx);
  for( i = 0; i < num_nodes; i++ ) {
    narr_node_idx_ptr[i] = 0;
  }
  solution->node_idx = narr_node_idx_ptr;

  return;
}

void solution__destroy( Solution *solution ) {
  xfree( solution->ids_shape );
  xfree( solution->node_idx_shape );
  xfree( solution );
  return;
}

void solution__gc_mark( Solution *solution ) {
  rb_gc_mark_movable(solution->narr_ids);
  rb_gc_mark_movable(solution->narr_node_idx);
  return;
}

void solution__gc_compact(Solution *solution) {
  solution->narr_ids = rb_gc_location(solution->narr_ids);
  solution->narr_node_idx = rb_gc_location(solution->narr_node_idx);
  return;
}

void solution__deep_copy( Solution *solution_copy, Solution *solution_orig ) {
  solution_copy->num_nodes = solution_orig->num_nodes;

  solution_copy->narr_ids = tsp_numo_clone(solution_orig->narr_ids);
  solution_copy->ids = (int32_t *)tsp_numo_read_write_pointer(solution_copy->narr_ids);
  solution_copy->ids_shape = ALLOC_N( int, 1 );
  memcpy( solution_copy->ids_shape, solution_orig->ids_shape, 1 * sizeof(int) );

  solution_copy->narr_node_idx = tsp_numo_clone(solution_orig->narr_node_idx);
  solution_copy->node_idx = (int32_t *)tsp_numo_read_write_pointer(solution_copy->narr_node_idx);
  solution_copy->node_idx_shape = ALLOC_N( int, 1 );
  memcpy( solution_copy->node_idx_shape, solution_orig->node_idx_shape, 1 * sizeof(int) );

  return;
}

Solution * solution__clone( Solution *solution_orig ) {
  Solution * solution_copy = solution__create();
  solution__deep_copy( solution_copy, solution_orig );
  return solution_copy;
}
