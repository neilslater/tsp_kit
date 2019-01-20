// ext/tsp_kit/struct_euclidean_nodes.c

#include "struct_euclidean_nodes.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for EuclideanNodes memory management
//

EuclideanNodes *euclidean_nodes__create() {
  EuclideanNodes *euclidean_nodes;
  euclidean_nodes = xmalloc( sizeof(EuclideanNodes) );
  euclidean_nodes->num_nodes = 0;
  euclidean_nodes->num_dims = 0;
  euclidean_nodes->locations_shape = NULL;
  euclidean_nodes->narr_locations = Qnil;
  euclidean_nodes->locations = NULL;
  return euclidean_nodes;
}

void euclidean_nodes__init( EuclideanNodes *euclidean_nodes, int num_nodes, int num_dims ) {
  int i;
  struct NARRAY *narr;
  double *narr_locations_ptr;

  euclidean_nodes->num_nodes = num_nodes;

  euclidean_nodes->num_dims = num_dims;

  euclidean_nodes->locations_shape = ALLOC_N( int, 2 );
  euclidean_nodes->locations_shape[0] = num_dims;
  euclidean_nodes->locations_shape[1] = num_nodes;
  euclidean_nodes->narr_locations = na_make_object( NA_DFLOAT, 2, euclidean_nodes->locations_shape, cNArray );
  GetNArray( euclidean_nodes->narr_locations, narr );
  narr_locations_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_locations_ptr[i] = 0.0;
  }
  euclidean_nodes->locations = (double *) narr->ptr;

  return;
}

void euclidean_nodes__destroy( EuclideanNodes *euclidean_nodes ) {
  xfree( euclidean_nodes->locations_shape );
  xfree( euclidean_nodes );
  return;
}

void euclidean_nodes__gc_mark( EuclideanNodes *euclidean_nodes ) {
  rb_gc_mark( euclidean_nodes->narr_locations );
  return;
}

void euclidean_nodes__deep_copy( EuclideanNodes *euclidean_nodes_copy, EuclideanNodes *euclidean_nodes_orig ) {
  struct NARRAY *narr;

  euclidean_nodes_copy->num_nodes = euclidean_nodes_orig->num_nodes;
  euclidean_nodes_copy->num_dims = euclidean_nodes_orig->num_dims;

  euclidean_nodes_copy->narr_locations = na_clone( euclidean_nodes_orig->narr_locations );
  GetNArray( euclidean_nodes_copy->narr_locations, narr );
  euclidean_nodes_copy->locations = (double *) narr->ptr;
  euclidean_nodes_copy->locations_shape = ALLOC_N( int, 2 );
  memcpy( euclidean_nodes_copy->locations_shape, euclidean_nodes_orig->locations_shape, 2 * sizeof(int) );

  return;
}

EuclideanNodes * euclidean_nodes__clone( EuclideanNodes *euclidean_nodes_orig ) {
  EuclideanNodes * euclidean_nodes_copy = euclidean_nodes__create();
  euclidean_nodes__deep_copy( euclidean_nodes_copy, euclidean_nodes_orig );
  return euclidean_nodes_copy;
}
