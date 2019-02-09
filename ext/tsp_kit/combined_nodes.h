// ext/tsp_kit/combined_nodes.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Some helper functions for handling switching between Nodes subclasses
//

#ifndef STRUCT_COMBINED_NODES_H
#define STRUCT_COMBINED_NODES_H

#include <ruby.h>
#include "narray.h"
#include "struct_cost_matrix.h"
#include "struct_euclidean_nodes.h"
#include "utilities.h"

double null_distance_fn( void *nodes, int node_a_id, int node_b_id );

void null_distance_array_fn( void *nodes, int node_id, double * distances );

typedef double (*DISTANCEFN)(void*, int, int);

typedef void (*DISTANCEARRAYFN)(void*, int, double*);

typedef enum node_subclass {NODE_EUC, NODE_CM} NodeType;

NodeType get_node_type_from_rv( VALUE rv_nodes );

DISTANCEFN distance_fn_of( NodeType node_type );

DISTANCEARRAYFN distance_array_fn_of( NodeType node_type );

#endif
