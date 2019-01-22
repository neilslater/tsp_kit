// ext/tsp_kit/struct_euclidean_nodes.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for EuclideanNodes and declarations for its memory management
//

#ifndef STRUCT_EUCLIDEAN_NODES_H
#define STRUCT_EUCLIDEAN_NODES_H

#include <ruby.h>
#include "narray.h"

typedef struct _euclidean_nodes_raw {
  int num_nodes;
  int num_dims;
  int *locations_shape;
  volatile VALUE narr_locations;
  double *locations;
  } EuclideanNodes;

EuclideanNodes *euclidean_nodes__create();

void euclidean_nodes__init( EuclideanNodes *euclidean_nodes, int num_nodes, int num_dims );

void euclidean_nodes__destroy( EuclideanNodes *euclidean_nodes );

void euclidean_nodes__gc_mark( EuclideanNodes *euclidean_nodes );

void euclidean_nodes__deep_copy( EuclideanNodes *euclidean_nodes_copy, EuclideanNodes *euclidean_nodes_orig );

EuclideanNodes * euclidean_nodes__clone( EuclideanNodes *euclidean_nodes_orig );

///////////////////////////////////////////////////////////////////////////////////////////////////

double euclidean_nodes__distance_between( void *nodes_addr, int node_a_id, int node_b_id );

#endif
