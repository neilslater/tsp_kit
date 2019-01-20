// ext/tsp_kit/struct_greedy_solver.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for GreedySolver and declarations for its memory management
//

#ifndef STRUCT_GREEDY_SOLVER_H
#define STRUCT_GREEDY_SOLVER_H

#include <ruby.h>
#include "narray.h"

typedef struct _greedy_solver_raw {
  int num_nodes;
  int max_section_id;
  int route_start;
  int count_links;
  int *route_link_left;
  int *route_link_right;
  int *path_section_id;
  } GreedySolver;

GreedySolver *greedy_solver__create();

void greedy_solver__init( GreedySolver *greedy_solver, int num_nodes );

void greedy_solver__destroy( GreedySolver *greedy_solver );

void greedy_solver__gc_mark( GreedySolver *greedy_solver );

void greedy_solver__deep_copy( GreedySolver *greedy_solver_copy, GreedySolver *greedy_solver_orig );

GreedySolver * greedy_solver__clone( GreedySolver *greedy_solver_orig );

#endif
