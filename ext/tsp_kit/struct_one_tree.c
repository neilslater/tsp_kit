// ext/tsp_kit/struct_one_tree.c

#include "struct_one_tree.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for OneTree memory management
//

OneTree *one_tree__create() {
  OneTree *tree;
  tree = xmalloc( sizeof(OneTree) );
  tree->num_nodes = 0;
  tree->node_penalties_shape = NULL;
  tree->narr_node_penalties = Qnil;
  tree->node_penalties = NULL;
  tree->node_ids_shape = NULL;
  tree->narr_node_ids = Qnil;
  tree->node_ids = NULL;
  tree->parents_shape = NULL;
  tree->narr_parents = Qnil;
  tree->parents = NULL;
  tree->q = NULL;
  tree->c = NULL;
  tree->d = NULL;
  tree->locations = NULL;
  tree->dr_max_rank = 0;
  tree->dr_closest_nodes = NULL;
  return tree;
}

void one_tree__init( OneTree *tree, int num_nodes ) {
  int i;
  struct NARRAY *narr;
  double *narr_node_penalties_ptr;
  int32_t *narr_node_ids_ptr;
  int32_t *narr_parents_ptr;

  tree->num_nodes = num_nodes;

  tree->dr_max_rank = -1;

  tree->q = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    tree->q[i] = 1;
  }

  tree->c = ALLOC_N( double, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    tree->c[i] = DBL_MAX;
  }

  tree->d = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    tree->d[i] = 0;
  }

  tree->locations = ALLOC_N( double, 20 );
  for( i = 0; i < 20; i++ ) {
    tree->locations[i] = -1;
  }

  tree->dr_closest_nodes = ALLOC_N( int, 20 );
  for( i = 0; i < 20; i++ ) {
    tree->dr_closest_nodes[i] = -1;
  }

  tree->node_penalties_shape = ALLOC_N( int, 1 );
  tree->node_penalties_shape[0] = num_nodes;
  tree->narr_node_penalties = na_make_object( NA_DFLOAT, 1, tree->node_penalties_shape, cNArray );
  GetNArray( tree->narr_node_penalties, narr );
  narr_node_penalties_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_penalties_ptr[i] = 0.0;
  }
  tree->node_penalties = (double *) narr->ptr;

  tree->node_ids_shape = ALLOC_N( int, 1 );
  tree->node_ids_shape[0] = num_nodes + 2;
  tree->narr_node_ids = na_make_object( NA_LINT, 1, tree->node_ids_shape, cNArray );
  GetNArray( tree->narr_node_ids, narr );
  narr_node_ids_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_ids_ptr[i] = 0;
  }
  tree->node_ids = (int32_t *) narr->ptr;

  tree->parents_shape = ALLOC_N( int, 1 );
  tree->parents_shape[0] = num_nodes + 2;
  tree->narr_parents = na_make_object( NA_LINT, 1, tree->parents_shape, cNArray );
  GetNArray( tree->narr_parents, narr );
  narr_parents_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_parents_ptr[i] = 0;
  }
  tree->parents = (int32_t *) narr->ptr;

  return;
}

void one_tree__destroy( OneTree *tree ) {
  xfree( tree->q );
  xfree( tree->c );
  xfree( tree->d );
  xfree( tree->locations );
  xfree( tree->dr_closest_nodes );
  xfree( tree->node_penalties_shape );
  xfree( tree->node_ids_shape );
  xfree( tree->parents_shape );
  xfree( tree );
  return;
}

void one_tree__gc_mark( OneTree *tree ) {
  rb_gc_mark( tree->narr_node_penalties );
  rb_gc_mark( tree->narr_node_ids );
  rb_gc_mark( tree->narr_parents );
  return;
}

void one_tree__deep_copy( OneTree *tree_copy, OneTree *tree_orig ) {
  struct NARRAY *narr;
  int num_nodes = tree_orig->num_nodes;

  tree_copy->num_nodes = num_nodes;
  tree_copy->dr_max_rank = tree_orig->dr_max_rank;

  tree_copy->narr_node_penalties = na_clone( tree_orig->narr_node_penalties );
  GetNArray( tree_copy->narr_node_penalties, narr );
  tree_copy->node_penalties = (double *) narr->ptr;
  tree_copy->node_penalties_shape = ALLOC_N( int, 1 );
  memcpy( tree_copy->node_penalties_shape, tree_orig->node_penalties_shape, 1 * sizeof(int) );

  tree_copy->narr_node_ids = na_clone( tree_orig->narr_node_ids );
  GetNArray( tree_copy->narr_node_ids, narr );
  tree_copy->node_ids = (int32_t *) narr->ptr;
  tree_copy->node_ids_shape = ALLOC_N( int, 1 );
  memcpy( tree_copy->node_ids_shape, tree_orig->node_ids_shape, 1 * sizeof(int) );

  tree_copy->narr_parents = na_clone( tree_orig->narr_parents );
  GetNArray( tree_copy->narr_parents, narr );
  tree_copy->parents = (int32_t *) narr->ptr;
  tree_copy->parents_shape = ALLOC_N( int, 1 );
  memcpy( tree_copy->parents_shape, tree_orig->parents_shape, 1 * sizeof(int) );

  tree_copy->q = ALLOC_N( int, num_nodes );
  memcpy( tree_copy->q, tree_orig->q, ( num_nodes ) * sizeof(int) );

  tree_copy->c = ALLOC_N( double, num_nodes );
  memcpy( tree_copy->c, tree_orig->c, ( num_nodes ) * sizeof(double) );

  tree_copy->d = ALLOC_N( int, num_nodes );
  memcpy( tree_copy->d, tree_orig->d, ( num_nodes ) * sizeof(int) );

  tree_copy->locations = ALLOC_N( double, 20 );
  memcpy( tree_copy->locations, tree_orig->locations, ( 20 ) * sizeof(double) );

  tree_copy->dr_closest_nodes = ALLOC_N( int, 20 );
  memcpy( tree_copy->dr_closest_nodes, tree_orig->dr_closest_nodes, ( 20 ) * sizeof(int) );

  return;
}

OneTree *one_tree__clone( OneTree *tree_orig ) {
  OneTree *tree_copy = one_tree__create();
  one_tree__deep_copy( tree_copy, tree_orig );
  return tree_copy;
}
