// ext/tsp_kit/struct_one_tree.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for OneTree and declarations for its memory management
//

#ifndef STRUCT_ONE_TREE_H
#define STRUCT_ONE_TREE_H

#include <float.h>

#include <ruby.h>
#include "narray.h"

typedef struct _one_tree_raw {
  int num_nodes;
  int *node_penalties_shape;
  volatile VALUE narr_node_penalties;
  double *node_penalties;
  int *node_ids_shape;
  volatile VALUE narr_node_ids;
  int32_t *node_ids;
  int *parents_shape;
  volatile VALUE narr_parents;
  int32_t *parents;
  int *q;
  double *c;
  int *d;
  double *locations;
  int dr_max_rank;
  int *dr_closest_nodes;
  } OneTree;

OneTree *one_tree__create();

void one_tree__init( OneTree *one_tree, int num_nodes );

void one_tree__destroy( OneTree *one_tree );

void one_tree__gc_mark( OneTree *one_tree );

void one_tree__deep_copy( OneTree *one_tree_copy, OneTree *one_tree_orig );

OneTree *one_tree__clone( OneTree *one_tree_orig );

#endif
