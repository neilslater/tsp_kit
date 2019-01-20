// ext/tsp_kit/struct_greedy_solver.c

#include "struct_greedy_solver.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for GreedySolver memory management
//

GreedySolver *greedy_solver__create() {
  GreedySolver *greedy_solver;
  greedy_solver = xmalloc( sizeof(GreedySolver) );
  greedy_solver->num_nodes = 0;
  greedy_solver->max_section_id = 0;
  greedy_solver->route_start = 0;
  greedy_solver->count_links = 0;
  greedy_solver->route_link_left = NULL;
  greedy_solver->route_link_right = NULL;
  greedy_solver->path_section_id = NULL;
  return greedy_solver;
}

void greedy_solver__init( GreedySolver *greedy_solver, int num_nodes ) {
  int i;

  greedy_solver->num_nodes = num_nodes;

  greedy_solver->max_section_id = 0;

  greedy_solver->route_start = -1;

  greedy_solver->count_links = 0;

  greedy_solver->route_link_left = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    greedy_solver->route_link_left[i] = -1;
  }

  greedy_solver->route_link_right = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    greedy_solver->route_link_right[i] = -1;
  }

  greedy_solver->path_section_id = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    greedy_solver->path_section_id[i] = -1;
  }

  return;
}

void greedy_solver__destroy( GreedySolver *greedy_solver ) {
  xfree( greedy_solver->route_link_left );
  xfree( greedy_solver->route_link_right );
  xfree( greedy_solver->path_section_id );
  xfree( greedy_solver );
  return;
}

void greedy_solver__gc_mark( GreedySolver *greedy_solver ) {
  return;
}

void greedy_solver__deep_copy( GreedySolver *greedy_solver_copy, GreedySolver *greedy_solver_orig ) {
  int num_nodes = greedy_solver_orig->num_nodes;

  greedy_solver_copy->num_nodes = num_nodes;
  greedy_solver_copy->max_section_id = greedy_solver_orig->max_section_id;
  greedy_solver_copy->route_start = greedy_solver_orig->route_start;
  greedy_solver_copy->count_links = greedy_solver_orig->count_links;

  greedy_solver_copy->route_link_left = ALLOC_N( int, num_nodes );
  memcpy( greedy_solver_copy->route_link_left, greedy_solver_orig->route_link_left, ( num_nodes ) * sizeof(int) );

  greedy_solver_copy->route_link_right = ALLOC_N( int, num_nodes );
  memcpy( greedy_solver_copy->route_link_right, greedy_solver_orig->route_link_right, ( num_nodes ) * sizeof(int) );

  greedy_solver_copy->path_section_id = ALLOC_N( int, num_nodes );
  memcpy( greedy_solver_copy->path_section_id, greedy_solver_orig->path_section_id, ( num_nodes ) * sizeof(int) );

  return;
}

GreedySolver * greedy_solver__clone( GreedySolver *greedy_solver_orig ) {
  GreedySolver * greedy_solver_copy = greedy_solver__create();
  greedy_solver__deep_copy( greedy_solver_copy, greedy_solver_orig );
  return greedy_solver_copy;
}
