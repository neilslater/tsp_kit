// ext/tsp_kit/struct_cost_matrix.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for CostMatrix and declarations for its memory management
//

#ifndef STRUCT_WEIGHT_MATRIX_H
#define STRUCT_WEIGHT_MATRIX_H

#include <ruby.h>
#include "narray.h"
#include <stdbool.h>

typedef struct _cost_matrix_raw {
  int num_nodes;
  int *weights_shape;
  volatile VALUE narr_weights;
  double *weights;
  } CostMatrix;

#include "struct_euclidean_nodes.h"

CostMatrix *cost_matrix__create();

void cost_matrix__init( CostMatrix *cost_matrix, int num_nodes );

void cost_matrix__destroy( CostMatrix *cost_matrix );

void cost_matrix__gc_mark( CostMatrix *cost_matrix );

void cost_matrix__deep_copy( CostMatrix *cost_matrix_copy, CostMatrix *cost_matrix_orig );

CostMatrix * cost_matrix__clone( CostMatrix *cost_matrix_orig );

////////////////////////////////////////////////////////////////////////////////////////////////////

bool cost_matrix__validate( CostMatrix *cost_matrix );

double cost_matrix__distance_between( void *nodes_addr, int node_a_id, int node_b_id );

void cost_matrix__all_distances_from( void *nodes_addr, int node_id, double *distances_buffer );

#endif
