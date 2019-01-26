// ext/tsp_kit/struct_weight_matrix.c

#include "struct_weight_matrix.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for WeightMatrix memory management
//

WeightMatrix *weight_matrix__create() {
  WeightMatrix *weight_matrix;
  weight_matrix = xmalloc( sizeof(WeightMatrix) );
  weight_matrix->num_nodes = 0;
  weight_matrix->weights_shape = NULL;
  weight_matrix->narr_weights = Qnil;
  weight_matrix->weights = NULL;
  return weight_matrix;
}

void weight_matrix__init( WeightMatrix *weight_matrix, int num_nodes ) {
  int i;
  struct NARRAY *narr;
  double *narr_weights_ptr;

  weight_matrix->num_nodes = num_nodes;

  weight_matrix->weights_shape = ALLOC_N( int, 2 );
  weight_matrix->weights_shape[0] = num_nodes;
  weight_matrix->weights_shape[1] = num_nodes;
  weight_matrix->narr_weights = na_make_object( NA_DFLOAT, 2, weight_matrix->weights_shape, cNArray );
  GetNArray( weight_matrix->narr_weights, narr );
  narr_weights_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_weights_ptr[i] = 0.0;
  }
  weight_matrix->weights = (double *) narr->ptr;

  return;
}

void weight_matrix__destroy( WeightMatrix *weight_matrix ) {
  xfree( weight_matrix->weights_shape );
  xfree( weight_matrix );
  return;
}

void weight_matrix__gc_mark( WeightMatrix *weight_matrix ) {
  rb_gc_mark( weight_matrix->narr_weights );
  return;
}

void weight_matrix__deep_copy( WeightMatrix *weight_matrix_copy, WeightMatrix *weight_matrix_orig ) {
  struct NARRAY *narr;

  weight_matrix_copy->num_nodes = weight_matrix_orig->num_nodes;

  weight_matrix_copy->narr_weights = na_clone( weight_matrix_orig->narr_weights );
  GetNArray( weight_matrix_copy->narr_weights, narr );
  weight_matrix_copy->weights = (double *) narr->ptr;
  weight_matrix_copy->weights_shape = ALLOC_N( int, 2 );
  memcpy( weight_matrix_copy->weights_shape, weight_matrix_orig->weights_shape, 2 * sizeof(int) );

  return;
}

WeightMatrix * weight_matrix__clone( WeightMatrix *weight_matrix_orig ) {
  WeightMatrix * weight_matrix_copy = weight_matrix__create();
  weight_matrix__deep_copy( weight_matrix_copy, weight_matrix_orig );
  return weight_matrix_copy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool weight_matrix__validate( WeightMatrix *weight_matrix ) {
  bool ok = true;
  int n = weight_matrix->num_nodes;
  double *w = weight_matrix->weights;

  for( int i = 0; i < (n - 1); i++ ) {
    if ( w[i * n + i] != 0.0 ) {
      ok = false;
      break;
    }

    for( int j = i + 1; j < weight_matrix->num_nodes; j++ ) {
      if ( w[i * n + j] != w[j * n + i] || w[i * n + j] < 0.0 ) {
        ok = false;
        break;
      }
    }

    if (! ok) break;
  }

  return ok;
}

double weight_matrix__distance_between( void *nodes_addr, int node_a_id, int node_b_id ) {
  WeightMatrix *nodes = (WeightMatrix *) nodes_addr;
  return nodes->weights[ node_a_id * nodes->num_nodes + node_b_id ];
}

void weight_matrix__all_distances_from( void *nodes_addr, int node_id, double *distances_buffer ) {
  WeightMatrix *nodes = (WeightMatrix *) nodes_addr;
  int numn = nodes->num_nodes;
  int offset = numn * node_id;

  for( int j = 0; j < numn; j++ ) {
    distances_buffer[j] = nodes->weights[ offset + j ];
  }

  return;
}
