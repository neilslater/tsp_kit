// ext/tsp_kit/struct_distance_rank.c

#include "struct_distance_rank.h"
#include "utilities.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for DistanceRank memory management
//

DistanceRank *distance_rank__create() {
  DistanceRank *distance_rank;
  distance_rank = xmalloc( sizeof(DistanceRank) );
  distance_rank->num_nodes = 0;
  distance_rank->max_rank = 0;
  distance_rank->closest_nodes_shape = NULL;
  distance_rank->narr_closest_nodes = Qnil;
  distance_rank->closest_nodes = NULL;
  return distance_rank;
}

void distance_rank__init( DistanceRank *distance_rank, int num_nodes, int max_rank ) {
  int i;
  struct NARRAY *narr;
  int32_t *narr_closest_nodes_ptr;

  distance_rank->num_nodes = num_nodes;

  distance_rank->max_rank = max_rank;

  distance_rank->closest_nodes_shape = ALLOC_N( int, 2 );
  distance_rank->closest_nodes_shape[0] = max_rank;
  distance_rank->closest_nodes_shape[1] = num_nodes;
  distance_rank->narr_closest_nodes = na_make_object( NA_LINT, 2, distance_rank->closest_nodes_shape, cNArray );
  GetNArray( distance_rank->narr_closest_nodes, narr );
  narr_closest_nodes_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_closest_nodes_ptr[i] = 0;
  }
  distance_rank->closest_nodes = (int32_t *) narr->ptr;

  return;
}

void distance_rank__destroy( DistanceRank *distance_rank ) {
  xfree( distance_rank->closest_nodes_shape );
  xfree( distance_rank );
  return;
}

void distance_rank__gc_mark( DistanceRank *distance_rank ) {
  rb_gc_mark( distance_rank->narr_closest_nodes );
  return;
}

void distance_rank__deep_copy( DistanceRank *distance_rank_copy, DistanceRank *distance_rank_orig ) {
  struct NARRAY *narr;

  distance_rank_copy->num_nodes = distance_rank_orig->num_nodes;
  distance_rank_copy->max_rank = distance_rank_orig->max_rank;

  distance_rank_copy->narr_closest_nodes = na_clone( distance_rank_orig->narr_closest_nodes );
  GetNArray( distance_rank_copy->narr_closest_nodes, narr );
  distance_rank_copy->closest_nodes = (int32_t *) narr->ptr;
  distance_rank_copy->closest_nodes_shape = ALLOC_N( int, 2 );
  memcpy( distance_rank_copy->closest_nodes_shape, distance_rank_orig->closest_nodes_shape, 2 * sizeof(int) );

  return;
}

DistanceRank * distance_rank__clone( DistanceRank *distance_rank_orig ) {
  DistanceRank * distance_rank_copy = distance_rank__create();
  distance_rank__deep_copy( distance_rank_copy, distance_rank_orig );
  return distance_rank_copy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

DistanceRank * distance_rank__from_euclidean_nodes( EuclideanNodes *nodes, int max_rank ) {
  int dr_offset;
  int *node_ids;
  int num_nodes = nodes->num_nodes;
  double *distances;
  DistanceRank *dr = distance_rank__create();
  distance_rank__init( dr, num_nodes, max_rank );
  node_ids = ALLOC_N( int, num_nodes );
  distances = ALLOC_N( double, num_nodes );

  for( int i = 0; i < num_nodes; i++ ) {
    node_ids[i] = i;
    distances[i] = 0.0;
  }

  for( int i = 0; i < num_nodes; i++ ) {
    dr_offset = max_rank * i;
    euclidean_nodes__all_distances_from( nodes, i, distances );
    quicksort_ids_by_double( node_ids, distances, 0, num_nodes - 1 );
    memcpy( dr->closest_nodes + dr_offset, node_ids + 1, max_rank * sizeof(int) );
    for( int j = max_rank; j > 0; j-- ) {
      if ( distances[ node_ids[j] ] > 1.0e300 ) {
        dr->closest_nodes[j-1] = -1;
      } else {
        break;
      }
    }
  }

  xfree( distances );
  xfree( node_ids );
  return dr;
}

DistanceRank * distance_rank__from_weight_matrix( WeightMatrix *nodes, int max_rank ) {
  int dr_offset;
  int *node_ids;
  int num_nodes = nodes->num_nodes;
  double *distances;
  DistanceRank *dr = distance_rank__create();
  distance_rank__init( dr, num_nodes, max_rank );
  node_ids = ALLOC_N( int, num_nodes );
  distances = ALLOC_N( double, num_nodes );

  for( int i = 0; i < num_nodes; i++ ) {
    node_ids[i] = i;
    distances[i] = 0.0;
  }

  for( int i = 0; i < num_nodes; i++ ) {
    dr_offset = max_rank * i;
    weight_matrix__all_distances_from( nodes, i, distances );
    quicksort_ids_by_double( node_ids, distances, 0, num_nodes - 1 );
    memcpy( dr->closest_nodes + dr_offset, node_ids + 1, max_rank * sizeof(int) );
    for( int j = max_rank; j > 0; j-- ) {
      if ( distances[ node_ids[j] ] > 1.0e300 ) {
        dr->closest_nodes[j-1] = -1;
      } else {
        break;
      }
    }
  }

  xfree( distances );
  xfree( node_ids );
  return dr;
}
