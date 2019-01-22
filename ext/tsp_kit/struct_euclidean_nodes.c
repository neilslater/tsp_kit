// ext/tsp_kit/struct_euclidean_nodes.c

#include "struct_euclidean_nodes.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for EuclideanNodes memory management
//

EuclideanNodes *euclidean_nodes__create() {
  EuclideanNodes *nodes;
  nodes = xmalloc( sizeof(EuclideanNodes) );
  nodes->num_nodes = 0;
  nodes->num_dims = 0;
  nodes->locations_shape = NULL;
  nodes->narr_locations = Qnil;
  nodes->locations = NULL;
  return nodes;
}

void euclidean_nodes__init( EuclideanNodes *nodes, int num_nodes, int num_dims ) {
  int i;
  struct NARRAY *narr;
  double *narr_locations_ptr;

  nodes->num_nodes = num_nodes;
  nodes->num_dims = num_dims;

  nodes->locations_shape = ALLOC_N( int, 2 );
  nodes->locations_shape[0] = num_dims;
  nodes->locations_shape[1] = num_nodes;
  nodes->narr_locations = na_make_object( NA_DFLOAT, 2, nodes->locations_shape, cNArray );
  GetNArray( nodes->narr_locations, narr );
  narr_locations_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_locations_ptr[i] = 0.0;
  }
  nodes->locations = (double *) narr->ptr;

  return;
}

void euclidean_nodes__destroy( EuclideanNodes *nodes ) {
  xfree( nodes->locations_shape );
  xfree( nodes );
  return;
}

void euclidean_nodes__gc_mark( EuclideanNodes *nodes ) {
  rb_gc_mark( nodes->narr_locations );
  return;
}

void euclidean_nodes__deep_copy( EuclideanNodes *nodes_copy, EuclideanNodes *nodes_orig ) {
  struct NARRAY *narr;

  nodes_copy->num_nodes = nodes_orig->num_nodes;
  nodes_copy->num_dims = nodes_orig->num_dims;

  nodes_copy->narr_locations = na_clone( nodes_orig->narr_locations );
  GetNArray( nodes_copy->narr_locations, narr );
  nodes_copy->locations = (double *) narr->ptr;
  nodes_copy->locations_shape = ALLOC_N( int, 2 );
  memcpy( nodes_copy->locations_shape, nodes_orig->locations_shape, 2 * sizeof(int) );

  return;
}

EuclideanNodes *euclidean_nodes__clone( EuclideanNodes *nodes_orig ) {
  EuclideanNodes *nodes_copy = euclidean_nodes__create();
  euclidean_nodes__deep_copy( nodes_copy, nodes_orig );
  return nodes_copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// User functions
//

// This takes void* first argument to fit with DISTANCEFN definition
double euclidean_nodes__distance_between( void *nodes_addr, int node_a_id, int node_b_id ) {
  EuclideanNodes *nodes = (EuclideanNodes *) nodes_addr;
  int numd = nodes->num_dims;

  double d2 = 0.0;
  for( int i = 0; i < numd; i++ ) {
    double dx = nodes->locations[i+numd*node_a_id] - nodes->locations[i+numd*node_b_id];
    d2 += dx * dx;
  }

  return sqrt(d2);
}
