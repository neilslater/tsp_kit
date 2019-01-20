// ext/tsp_kit/struct_solution.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for Solution and declarations for its memory management
//

#ifndef STRUCT_SOLUTION_H
#define STRUCT_SOLUTION_H

#include <ruby.h>
#include "narray.h"

typedef struct _solution_raw {
  int num_nodes;
  int *ids_shape;
  volatile VALUE narr_ids;
  int32_t *ids;
  int *node_idx_shape;
  volatile VALUE narr_node_idx;
  int32_t *node_idx;
  } Solution;

Solution *solution__create();

void solution__init( Solution *solution, int num_nodes );

void solution__destroy( Solution *solution );

void solution__gc_mark( Solution *solution );

void solution__deep_copy( Solution *solution_copy, Solution *solution_orig );

Solution * solution__clone( Solution *solution_orig );

#endif
