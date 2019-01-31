// ext/tsp_kit/combined_nodes.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Some helper functions for handling switching between Nodes subclasses
//

#ifndef STRUCT_COMBINED_NODES_H
#define STRUCT_COMBINED_NODES_H

#include <ruby.h>
#include "narray.h"
#include "struct_weight_matrix.h"
#include "struct_euclidean_nodes.h"
#include "utilities.h"

typedef double (*DISTANCEFN)(void*, int, int);

typedef void (*DISTANCEARRAYFN)(void*, int, double*);

typedef enum node_subclass {NODE_EUC, NODE_WM} NodeType;

NodeType get_node_type_from_rv( VALUE rv_nodes );

#endif
