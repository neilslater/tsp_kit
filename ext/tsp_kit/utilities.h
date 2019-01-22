// ext/stolen_sleigh/utilities.h

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Declarations of narray helper functions
//

#ifndef CORE_UTILITIES_H
#define CORE_UTILITIES_H

#include <math.h>
#include <stdbool.h>
#include "mt.h"

void quicksort_ids_by_double( int * ids, double * sort_by, int lowest, int highest );
void quicksort_ints( int * ids, int lowest, int highest );
void shuffle_ints( int n, int *array );

////////////////////////////////////////////////////////////////////////////////////////////////////

typedef double (*DISTANCEFN)(int,int,void*);

double distance_between_2d( int node_a_id, int node_b_id, double * locations );
double squared_distance_between_2d( int node_a_id, int node_b_id, double * locations );
void all_squared_distances_from_2d( int node_id, int num_nodes, double * locations, double * squared_distances_buffer );
void all_nearest_nodes( int node_id, int num_nodes, double * locations, double * squared_distances_buffer, int * nodes_buffer );
double segment_length( int n, int * segment_node_ids, double * locations );
double loop_length( int n, int * segment_node_ids, double * locations );
int nearby_node( int node_id, int n, int dr_max_rank, int *dr_closest_nodes );

#endif
