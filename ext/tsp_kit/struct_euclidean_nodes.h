// ext/tsp_kit/struct_euclidean_nodes.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for EuclideanNodes and declarations for its memory management
//

#ifndef STRUCT_EUCLIDEAN_NODES_H
#define STRUCT_EUCLIDEAN_NODES_H

#include <ruby.h>
#include "narray.h"
#include "struct_cost_matrix.h"

typedef struct _euclidean_nodes_raw {
  int num_nodes;
  int num_dims;
  int *locations_shape;
  volatile VALUE narr_locations;
  double *locations;
  } EuclideanNodes;

EuclideanNodes *euclidean_nodes__create();

void euclidean_nodes__init( EuclideanNodes *nodes, int num_nodes, int num_dims );

void euclidean_nodes__destroy( EuclideanNodes *nodes );

void euclidean_nodes__gc_mark( EuclideanNodes *nodes );

void euclidean_nodes__deep_copy( EuclideanNodes *nodes_copy, EuclideanNodes *nodes_orig );

EuclideanNodes * euclidean_nodes__clone( EuclideanNodes *nodes_orig );

///////////////////////////////////////////////////////////////////////////////////////////////////

double euclidean_nodes__distance_between( void *nodes_addr, int node_a_id, int node_b_id );

void euclidean_nodes__all_distances_from( void *nodes_addr, int node_id, double *distances_buffer );

CostMatrix *euclidean_nodes__create_cost_matrix( EuclideanNodes *nodes );

#endif
