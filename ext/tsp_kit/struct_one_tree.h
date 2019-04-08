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
#include "combined_nodes.h"

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
  int *tree_idx_of_node;
  int *best_penalty_node_id;
  int *second_best_penalty_node_id;
  double *second_best_penalty_distance;
  int current_tree_idx;
  int *d;

  int dr_max_rank;
  int *dr_closest_nodes;
  void *nodes;
  DISTANCEFN distance_fn;
  } OneTree;

#include "utilities.h"
#include "mt.h"
#include "struct_distance_rank.h"
#include "struct_priority_queue.h"

OneTree *one_tree__create();

void one_tree__init( OneTree *tree, int num_nodes );

void one_tree__destroy( OneTree *tree );

void one_tree__gc_mark( OneTree *tree );

void one_tree__deep_copy( OneTree *tree_copy, OneTree *tree_orig );

OneTree *one_tree__clone( OneTree *tree_orig );

///////////////////////////////////////////////////////////////////////////////////////////////////

void one_tree__build( OneTree *tree, NodeType ntype, void *nodes, DistanceRank *dr, int start_city_id );

double one_tree__length( OneTree *tree, NodeType ntype, void *nodes );

double one_tree__w_length( OneTree *tree, NodeType ntype, void *nodes );

void one_tree__calc_degrees( OneTree *tree );

void one_tree__reindex( OneTree *tree );

void one_tree__generate_alpha_ranking( OneTree *tree, NodeType ntype, void *nodes, DistanceRank *target_dr, int use_penalties );

#endif
