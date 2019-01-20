// ext/tsp_kit/struct_distance_rank.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for DistanceRank and declarations for its memory management
//

#ifndef STRUCT_DISTANCE_RANK_H
#define STRUCT_DISTANCE_RANK_H

#include <ruby.h>
#include "narray.h"

typedef struct _distance_rank_raw {
  int num_nodes;
  int max_rank;
  int *closest_nodes_shape;
  volatile VALUE narr_closest_nodes;
  int32_t *closest_nodes;
  } DistanceRank;

DistanceRank *distance_rank__create();

void distance_rank__init( DistanceRank *distance_rank, int num_nodes, int max_rank );

void distance_rank__destroy( DistanceRank *distance_rank );

void distance_rank__gc_mark( DistanceRank *distance_rank );

void distance_rank__deep_copy( DistanceRank *distance_rank_copy, DistanceRank *distance_rank_orig );

DistanceRank * distance_rank__clone( DistanceRank *distance_rank_orig );

#endif
