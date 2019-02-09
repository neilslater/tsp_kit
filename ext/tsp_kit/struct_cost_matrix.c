// ext/tsp_kit/struct_cost_matrix.c

#include "struct_cost_matrix.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for CostMatrix memory management
//

CostMatrix *cost_matrix__create() {
  CostMatrix *cost_matrix;
  cost_matrix = xmalloc( sizeof(CostMatrix) );
  cost_matrix->num_nodes = 0;
  cost_matrix->weights_shape = NULL;
  cost_matrix->narr_weights = Qnil;
  cost_matrix->weights = NULL;
  return cost_matrix;
}

void cost_matrix__init( CostMatrix *cost_matrix, int num_nodes ) {
  int i;
  struct NARRAY *narr;
  double *narr_weights_ptr;

  cost_matrix->num_nodes = num_nodes;

  cost_matrix->weights_shape = ALLOC_N( int, 2 );
  cost_matrix->weights_shape[0] = num_nodes;
  cost_matrix->weights_shape[1] = num_nodes;
  cost_matrix->narr_weights = na_make_object( NA_DFLOAT, 2, cost_matrix->weights_shape, cNArray );
  GetNArray( cost_matrix->narr_weights, narr );
  narr_weights_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_weights_ptr[i] = 0.0;
  }
  cost_matrix->weights = (double *) narr->ptr;

  return;
}

void cost_matrix__destroy( CostMatrix *cost_matrix ) {
  xfree( cost_matrix->weights_shape );
  xfree( cost_matrix );
  return;
}

void cost_matrix__gc_mark( CostMatrix *cost_matrix ) {
  rb_gc_mark( cost_matrix->narr_weights );
  return;
}

void cost_matrix__deep_copy( CostMatrix *cost_matrix_copy, CostMatrix *cost_matrix_orig ) {
  struct NARRAY *narr;

  cost_matrix_copy->num_nodes = cost_matrix_orig->num_nodes;

  cost_matrix_copy->narr_weights = na_clone( cost_matrix_orig->narr_weights );
  GetNArray( cost_matrix_copy->narr_weights, narr );
  cost_matrix_copy->weights = (double *) narr->ptr;
  cost_matrix_copy->weights_shape = ALLOC_N( int, 2 );
  memcpy( cost_matrix_copy->weights_shape, cost_matrix_orig->weights_shape, 2 * sizeof(int) );

  return;
}

CostMatrix * cost_matrix__clone( CostMatrix *cost_matrix_orig ) {
  CostMatrix * cost_matrix_copy = cost_matrix__create();
  cost_matrix__deep_copy( cost_matrix_copy, cost_matrix_orig );
  return cost_matrix_copy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool cost_matrix__validate( CostMatrix *cost_matrix ) {
  bool ok = true;
  int n = cost_matrix->num_nodes;
  double *w = cost_matrix->weights;

  for( int i = 0; i < (n - 1); i++ ) {
    if ( w[i * n + i] != 0.0 ) {
      ok = false;
      break;
    }

    for( int j = i + 1; j < cost_matrix->num_nodes; j++ ) {
      if ( w[i * n + j] != w[j * n + i] || w[i * n + j] < 0.0 ) {
        ok = false;
        break;
      }
    }

    if (! ok) break;
  }

  return ok;
}

double cost_matrix__distance_between( void *nodes_addr, int node_a_id, int node_b_id ) {
  CostMatrix *nodes = (CostMatrix *) nodes_addr;
  return nodes->weights[ node_a_id * nodes->num_nodes + node_b_id ];
}

void cost_matrix__all_distances_from( void *nodes_addr, int node_id, double *distances_buffer ) {
  CostMatrix *nodes = (CostMatrix *) nodes_addr;
  int numn = nodes->num_nodes;
  int offset = numn * node_id;

  for( int j = 0; j < numn; j++ ) {
    distances_buffer[j] = nodes->weights[ offset + j ];
  }

  return;
}
