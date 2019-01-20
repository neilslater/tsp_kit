// ext/tsp_kit/struct_one_tree.c

#include "struct_one_tree.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for OneTree memory management
//

OneTree *one_tree__create() {
  OneTree *one_tree;
  one_tree = xmalloc( sizeof(OneTree) );
  one_tree->num_nodes = 0;
  one_tree->node_penalties_shape = NULL;
  one_tree->narr_node_penalties = Qnil;
  one_tree->node_penalties = NULL;
  one_tree->node_ids_shape = NULL;
  one_tree->narr_node_ids = Qnil;
  one_tree->node_ids = NULL;
  one_tree->parents_shape = NULL;
  one_tree->narr_parents = Qnil;
  one_tree->parents = NULL;
  one_tree->q = NULL;
  one_tree->c = NULL;
  one_tree->d = NULL;
  one_tree->locations = NULL;
  one_tree->dr_max_rank = 0;
  one_tree->dr_closest_nodes = NULL;
  return one_tree;
}

void one_tree__init( OneTree *one_tree, int num_nodes ) {
  int i;
  struct NARRAY *narr;
  double *narr_node_penalties_ptr;
  int32_t *narr_node_ids_ptr;
  int32_t *narr_parents_ptr;

  one_tree->num_nodes = num_nodes;

  one_tree->dr_max_rank = -1;

  one_tree->q = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    one_tree->q[i] = 1;
  }

  one_tree->c = ALLOC_N( double, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    one_tree->c[i] = DBL_MAX;
  }

  one_tree->d = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    one_tree->d[i] = 0;
  }

  one_tree->locations = ALLOC_N( double, 20 );
  for( i = 0; i < 20; i++ ) {
    one_tree->locations[i] = -1;
  }

  one_tree->dr_closest_nodes = ALLOC_N( int, 20 );
  for( i = 0; i < 20; i++ ) {
    one_tree->dr_closest_nodes[i] = -1;
  }

  one_tree->node_penalties_shape = ALLOC_N( int, 1 );
  one_tree->node_penalties_shape[0] = num_nodes;
  one_tree->narr_node_penalties = na_make_object( NA_DFLOAT, 1, one_tree->node_penalties_shape, cNArray );
  GetNArray( one_tree->narr_node_penalties, narr );
  narr_node_penalties_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_penalties_ptr[i] = 0.0;
  }
  one_tree->node_penalties = (double *) narr->ptr;

  one_tree->node_ids_shape = ALLOC_N( int, 1 );
  one_tree->node_ids_shape[0] = num_nodes + 2;
  one_tree->narr_node_ids = na_make_object( NA_LINT, 1, one_tree->node_ids_shape, cNArray );
  GetNArray( one_tree->narr_node_ids, narr );
  narr_node_ids_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_ids_ptr[i] = 0;
  }
  one_tree->node_ids = (int32_t *) narr->ptr;

  one_tree->parents_shape = ALLOC_N( int, 1 );
  one_tree->parents_shape[0] = num_nodes + 2;
  one_tree->narr_parents = na_make_object( NA_LINT, 1, one_tree->parents_shape, cNArray );
  GetNArray( one_tree->narr_parents, narr );
  narr_parents_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_parents_ptr[i] = 0;
  }
  one_tree->parents = (int32_t *) narr->ptr;

  return;
}

void one_tree__destroy( OneTree *one_tree ) {
  xfree( one_tree->q );
  xfree( one_tree->c );
  xfree( one_tree->d );
  xfree( one_tree->locations );
  xfree( one_tree->dr_closest_nodes );
  xfree( one_tree->node_penalties_shape );
  xfree( one_tree->node_ids_shape );
  xfree( one_tree->parents_shape );
  xfree( one_tree );
  return;
}

void one_tree__gc_mark( OneTree *one_tree ) {
  rb_gc_mark( one_tree->narr_node_penalties );
  rb_gc_mark( one_tree->narr_node_ids );
  rb_gc_mark( one_tree->narr_parents );
  return;
}

void one_tree__deep_copy( OneTree *one_tree_copy, OneTree *one_tree_orig ) {
  struct NARRAY *narr;
  int num_nodes = one_tree_orig->num_nodes;

  one_tree_copy->num_nodes = num_nodes;
  one_tree_copy->dr_max_rank = one_tree_orig->dr_max_rank;

  one_tree_copy->narr_node_penalties = na_clone( one_tree_orig->narr_node_penalties );
  GetNArray( one_tree_copy->narr_node_penalties, narr );
  one_tree_copy->node_penalties = (double *) narr->ptr;
  one_tree_copy->node_penalties_shape = ALLOC_N( int, 1 );
  memcpy( one_tree_copy->node_penalties_shape, one_tree_orig->node_penalties_shape, 1 * sizeof(int) );

  one_tree_copy->narr_node_ids = na_clone( one_tree_orig->narr_node_ids );
  GetNArray( one_tree_copy->narr_node_ids, narr );
  one_tree_copy->node_ids = (int32_t *) narr->ptr;
  one_tree_copy->node_ids_shape = ALLOC_N( int, 1 );
  memcpy( one_tree_copy->node_ids_shape, one_tree_orig->node_ids_shape, 1 * sizeof(int) );

  one_tree_copy->narr_parents = na_clone( one_tree_orig->narr_parents );
  GetNArray( one_tree_copy->narr_parents, narr );
  one_tree_copy->parents = (int32_t *) narr->ptr;
  one_tree_copy->parents_shape = ALLOC_N( int, 1 );
  memcpy( one_tree_copy->parents_shape, one_tree_orig->parents_shape, 1 * sizeof(int) );

  one_tree_copy->q = ALLOC_N( int, num_nodes );
  memcpy( one_tree_copy->q, one_tree_orig->q, ( num_nodes ) * sizeof(int) );

  one_tree_copy->c = ALLOC_N( double, num_nodes );
  memcpy( one_tree_copy->c, one_tree_orig->c, ( num_nodes ) * sizeof(double) );

  one_tree_copy->d = ALLOC_N( int, num_nodes );
  memcpy( one_tree_copy->d, one_tree_orig->d, ( num_nodes ) * sizeof(int) );

  one_tree_copy->locations = ALLOC_N( double, 20 );
  memcpy( one_tree_copy->locations, one_tree_orig->locations, ( 20 ) * sizeof(double) );

  one_tree_copy->dr_closest_nodes = ALLOC_N( int, 20 );
  memcpy( one_tree_copy->dr_closest_nodes, one_tree_orig->dr_closest_nodes, ( 20 ) * sizeof(int) );

  return;
}

OneTree * one_tree__clone( OneTree *one_tree_orig ) {
  OneTree * one_tree_copy = one_tree__create();
  one_tree__deep_copy( one_tree_copy, one_tree_orig );
  return one_tree_copy;
}
