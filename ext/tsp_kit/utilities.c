// ext/tsp_kit/utilities.c

#include "utilities.h"

void quicksort_ids_by_double( int * ids, double * sort_by, int lowest, int highest ) {
  int pivot, j, i;
  int temp_id;

  if ( lowest < highest ) {
    pivot = lowest;
    i = lowest;
    j = highest;

    // Calculate pivot
    while ( i < j ) {
      while ( sort_by[ids[i]] <= sort_by[ids[pivot]] && i < highest )
        i++;
      while ( sort_by[ids[j]] > sort_by[ids[pivot]] )
        j--;
      if( i < j ) {
        temp_id = ids[i];
        ids[i] = ids[j];
        ids[j] = temp_id;
      }
    }

    temp_id = ids[pivot];
    ids[pivot] = ids[j];
    ids[j] = temp_id;

    quicksort_ids_by_double( ids, sort_by, lowest, j-1 );
    quicksort_ids_by_double( ids, sort_by, j+1, highest );
  }
}

void quicksort_ints( int * ids, int lowest, int highest ) {
  int pivot, j, i;
  int temp_id;

  if ( lowest < highest ) {
    pivot = lowest;
    i = lowest;
    j = highest;

    while ( i < j ) {
      while ( ids[i] <= ids[pivot] && i < highest )
        i++;
      while ( ids[j] > ids[pivot] )
        j--;
      if( i < j ) {
        temp_id = ids[i];
        ids[i] = ids[j];
        ids[j] = temp_id;
      }
    }

    temp_id = ids[pivot];
    ids[pivot] = ids[j];
    ids[j] = temp_id;

    quicksort_ints( ids, lowest, j-1 );
    quicksort_ints( ids, j+1, highest );
  }
}

// A Fisher-Yates shuffle
void shuffle_ints( int n, int *array ) {
  int i, tmp, r;
  for ( i = n-1; i >= 0; i-- ) {
    // This will be slightly biased for large n, but it is not a
    // noticeable issue for sizes of arrays used in TSP
    r = genrand_int31() % ( i + 1 );
    tmp = array[r];
    array[r] = array[i];
    array[i] = tmp;
  }
  return;
}

// Changes distance ranks for from_node_id so that to_node_id is first or second choice
/*
void preferred_distance_rank_link( int from_node_id, int to_node_id ) {
  int i, j;
  int *from_node_row = p_closest_nodes + from_node_id * cc_max_rank;
  if ( from_node_row[0] == to_node_id || from_node_row[1] == to_node_id ) {
    return;
  }

  j = 0;
  for( i = 2; i < cc_max_rank; i++ ) {
    if ( from_node_row[i] == to_node_id ) {
      j = i;
      break;
    }
  }

  if (! j) {
    j = cc_max_rank-1;
  }
  for( i = j; i > 0; i-- ) {
    from_node_row[i] = from_node_row[i-1];
  }
  from_node_row[0] = to_node_id;

  return;
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////

double distance_between_2d( int node_a_id, int node_b_id, double * locations ) {
  double dx = locations[2*node_a_id] - locations[2*node_b_id];
  double dy = locations[1+2*node_a_id] - locations[1+2*node_b_id];
  return sqrt(dx*dx + dy*dy);
}

double squared_distance_between_2d( int node_a_id, int node_b_id, double * locations ) {
  double dx = locations[2*node_a_id] - locations[2*node_b_id];
  double dy = locations[1+2*node_a_id] - locations[1+2*node_b_id];
  return dx*dx + dy*dy;
}

void all_squared_distances_from_2d( int node_id,  int num_nodes, double * locations, double * squared_distances_buffer ) {
  double sx = locations[2*node_id];
  double sy = locations[1+2*node_id];
  double dx, dy;
  for(int i=0; i < num_nodes; i++) {
    dx = sx - locations[2*i];
    dy = sy - locations[1+2*i];
    squared_distances_buffer[i] = dx*dx + dy*dy;
  }
  return;
}

void all_nearest_nodes( int node_id, int num_nodes, double *locations, double *squared_distances_buffer, int *nodes_buffer ) {
  all_squared_distances_from_2d( node_id, num_nodes, locations, squared_distances_buffer );
  quicksort_ids_by_double( nodes_buffer, squared_distances_buffer, 0, num_nodes - 1 );
  return;
}

double segment_length( int n, int * segment_node_ids, double * locations ) {
  double d = 0.0;
  int i;
  for(i = 0; i < n-1; i++) {
    d += distance_between_2d( segment_node_ids[i], segment_node_ids[i+1], locations );
  }
  return d;
}

double loop_length( int n, int * segment_node_ids, double * locations ) {
  return segment_length( n, segment_node_ids, locations )
              + distance_between_2d( segment_node_ids[n-1], segment_node_ids[0], locations );
}

int nearby_node( int node_id, int n, int dr_max_rank, int *dr_closest_nodes ) {
  return dr_closest_nodes[node_id * dr_max_rank + n];
}
