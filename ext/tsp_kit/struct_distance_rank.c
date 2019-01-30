// ext/tsp_kit/struct_distance_rank.c

#include "struct_distance_rank.h"

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
    narr_closest_nodes_ptr[i] = -1;
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

void distance_rank__resize( DistanceRank *distance_rank, int new_max_rank ) {
  int i, j, nn, copy_size;
  int *new_cn_ptr;
  struct NARRAY *narr;
  VALUE new_cn;

  nn = distance_rank->num_nodes;
  distance_rank->closest_nodes_shape[0] = new_max_rank;
  new_cn = na_make_object( NA_LINT, 2, distance_rank->closest_nodes_shape, cNArray );

  GetNArray( new_cn, narr );
  new_cn_ptr = (int32_t*) narr->ptr;

  copy_size = new_max_rank;
  if (new_max_rank > distance_rank->max_rank) {
    copy_size = distance_rank->max_rank;
  }

  for( i = 0; i < nn; i++ ) {
    memcpy( (new_cn_ptr + i * new_max_rank), (distance_rank->closest_nodes + i * distance_rank->max_rank), copy_size * sizeof(int32_t) );
  }

  // We *could* recalculate distances here, but user can always do that. Instead we add empty space.
  if (new_max_rank > distance_rank->max_rank) {
    for( i = 0; i < nn; i++ ) {
      for( j = copy_size; j < new_max_rank; j++ ) {
        new_cn_ptr[i * new_max_rank + j] = -1;
      }
    }
  }

  distance_rank->max_rank = new_max_rank;
  distance_rank->narr_closest_nodes = new_cn;
  distance_rank->closest_nodes = new_cn_ptr;

  return;
}

// This is called in a loop by another method that ensures we have enough size
int distance_rank__reciprocate( DistanceRank *distance_rank, void * nodes, DISTANCEFN dfunc, int rank_cutoff, int max_ranks_buffer ) {
  int i, j, n, m, p, q;
  bool found;
  int *k;
  int *matrix;
  int max_max_rank = rank_cutoff;
  double * distances;

  // Resize to buffer
  distance_rank__resize( distance_rank, max_ranks_buffer );

  n = distance_rank->num_nodes;
  m = distance_rank->max_rank;
  matrix = distance_rank->closest_nodes;
  distances = ALLOC_N( double, n );

  // Clear out any "extras"
  for(i = 0; i < n; i++) {
    for(j = rank_cutoff; j < m; j++) {
      matrix[i*m + j] = -1;
    }
  }

  k = ALLOC_N( int, n );
  for(i = 0; i < n; i++) {
    for(j = 0; j < rank_cutoff; j++) {
      if (matrix[i*m + j] < 0 ) break;
    }
    k[i] = j; // This is first -1 entry for node i, which may be less than the reciprocate limit
  }

  for(i = 0; i < n; i++) {
    for(j = 0; j < k[i]; j++) {
      // i mentions p . . .
      p = matrix[i*m + j];

      // Does p mention i back?
      found = false;
      for(q = 0; q < m; q++) {
        if (matrix[p*m + q] == i) {
          found = true;
          break;
        }
      }
      if (! found) {
        // No, so add to p's list
        if (k[p] < m) {
          matrix[p*m + k[p]] = i;
        }
        k[p]++;
        if (k[p] > max_max_rank) {
          max_max_rank = k[p];
        }
      }
    }

    // TODO: Some kind of callback?
    /*
    if ((i % 100) == 0) {
      printf(" Reciprocating minimum %d links. City %d, max rank required so far %d   \r", rank_cutoff, i, max_max_rank);
      fflush(stdout);
    }
    */
  }

  // Sort each row in distance order
  for(i = 0; i < n; i++) {
    for(j = 0; j < k[i]; j++) {
      p = matrix[i*m + j];
      distances[p] = (*dfunc)( nodes, i, p );
    }
    quicksort_ids_by_double( (matrix + i*m), distances, 0, k[i] - 1 );
  }

  xfree(k);
  xfree(distances);

  // printf("Reciprocating minimum %d links. Completed, max rank required %d                    \n", rank_cutoff, max_max_rank);

  return max_max_rank;
}

void distance_rank__bidirectional( DistanceRank *distance_rank, void * nodes, DISTANCEFN dfunc, int rank_cutoff ) {
  int max_ranks_buffer, needed_max_ranks;

  if (rank_cutoff >= distance_rank->num_nodes) {
    rank_cutoff = distance_rank->num_nodes - 1;
  }

  max_ranks_buffer = rank_cutoff * 2;
  if (max_ranks_buffer >= distance_rank->num_nodes) {
    max_ranks_buffer = distance_rank->num_nodes - 1;
  }

  needed_max_ranks = distance_rank__reciprocate( distance_rank, nodes, dfunc, rank_cutoff, max_ranks_buffer );

  // Downsize
  if (needed_max_ranks < max_ranks_buffer) {
    distance_rank__resize( distance_rank, needed_max_ranks );
  }

  // Re-do at correct larger size
  if (needed_max_ranks > max_ranks_buffer) {
    distance_rank__reciprocate( distance_rank, nodes, dfunc, rank_cutoff, needed_max_ranks );
  }

  return;
}
