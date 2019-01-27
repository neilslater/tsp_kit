// ext/tsp_kit/ruby_class_euclidean_nodes.c

#include "ruby_class_euclidean_nodes.h"
#include "ruby_class_weight_matrix.h"
#include "ruby_class_distance_rank.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_euclidean_nodes.c
//

VALUE euclidean_nodes_as_ruby_class( EuclideanNodes *euclidean_nodes , VALUE klass ) {
  return Data_Wrap_Struct( klass, euclidean_nodes__gc_mark, euclidean_nodes__destroy, euclidean_nodes );
}

VALUE euclidean_nodes_alloc(VALUE klass) {
  return euclidean_nodes_as_ruby_class( euclidean_nodes__create(), klass );
}

EuclideanNodes *get_euclidean_nodes_struct( VALUE obj ) {
  EuclideanNodes *euclidean_nodes;
  Data_Get_Struct( obj, EuclideanNodes, euclidean_nodes );
  return euclidean_nodes;
}

void assert_value_wraps_euclidean_nodes( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)euclidean_nodes__destroy) {
    rb_raise( rb_eTypeError, "Expected a EuclideanNodes object, but got something else" );
  }
}

/* Document-class: TspKit::EuclideanNodes
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes, num_dims )
 * Creates a new TspKit::Nodes::Euclidean
 * @param [Integer] num_nodes ...
 * @param [Integer] num_dims ...
 * @return [TspKit::Nodes::Euclidean] new ...
 */
VALUE euclidean_nodes_rbobject__initialize( VALUE self, VALUE rv_num_nodes, VALUE rv_num_dims ) {
  int num_nodes, num_dims;
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );
  num_nodes = NUM2INT( rv_num_nodes );
  if (num_nodes < 3 || num_nodes > 10000000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..10000000", num_nodes);
  }

  num_dims = NUM2INT( rv_num_dims );
  if (num_dims < 2 || num_dims > 10) {
    rb_raise(rb_eArgError, "num_dims %d is outside accepted range 2..10", num_dims);
  }

  euclidean_nodes__init( euclidean_nodes, num_nodes, num_dims );

  return self;
}

/* @overload clone
 * When cloned, the returned object has deep copies of all internal data.
 * @return [TspKit::Nodes::Euclidean] copy
 */
VALUE euclidean_nodes_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  EuclideanNodes *euclidean_nodes_copy;
  EuclideanNodes *euclidean_nodes_orig;

  if (copy == orig) return copy;
  euclidean_nodes_orig = get_euclidean_nodes_struct( orig );
  euclidean_nodes_copy = get_euclidean_nodes_struct( copy );

  euclidean_nodes__deep_copy( euclidean_nodes_copy, euclidean_nodes_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Number of nodes described by this collection, from 3 to 10000000.
 * @return [Integer]
 */
VALUE euclidean_nodes_rbobject__get_num_nodes( VALUE self ) {
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );
  return INT2NUM( euclidean_nodes->num_nodes );
}

/* @!attribute [r] num_dims
 * Number of spatial dimensions in the problem, from 2 to 2 (more later!)
 * @return [Integer]
 */
VALUE euclidean_nodes_rbobject__get_num_dims( VALUE self ) {
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );
  return INT2NUM( euclidean_nodes->num_dims );
}

/* @!attribute [r] locations
 * Description goes here
 * @return [NArray<float>]
 */
VALUE euclidean_nodes_rbobject__get_narr_locations( VALUE self ) {
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );
  return euclidean_nodes->narr_locations;
}

/* @overload from_data( locations )
 * Creates new TspKit::Nodes::Euclidean object directly from NArray of locations
 *
 * @return [TspKit::Nodes::Euclidean] new instance
 */
VALUE euclidean_nodes_rbclass__from_data( VALUE self, VALUE rv_locations) {
  struct NARRAY *narr;
  int num_nodes, num_dims;
  VALUE rv_nodes;
  EuclideanNodes *nodes;

  rv_locations = na_cast_object(rv_locations, NA_DFLOAT);
  GetNArray( rv_locations, narr );
  if (narr->rank != 2) {
    rb_raise(rb_eArgError, "locations array should have rank 2, but is rank %d", narr->rank);
  }

  num_dims = narr->shape[0];
  num_nodes = narr->shape[1];

  if (num_nodes < 3 || num_nodes > 10000000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..10000000", num_nodes);
  }

  if (num_dims < 2 || num_dims > 10) {
    rb_raise(rb_eArgError, "num_dims %d is outside accepted range 2..10", num_dims);
  }

  rv_nodes = euclidean_nodes_alloc( TspKit_EuclideanNodes );
  nodes = get_euclidean_nodes_struct( rv_nodes );
  nodes->num_nodes = num_nodes;
  nodes->num_dims = num_dims;
  nodes->narr_locations = rv_locations;
  nodes->locations = (double *) narr->ptr;
  nodes->locations_shape = ALLOC_N( int, 2 );
  nodes->locations_shape[0] = num_dims;
  nodes->locations_shape[1] = num_nodes;
  return rv_nodes;
}

/* @overload distance_between( node_a_id, node_b_id )
 * Returns distance metric between two nodes.
 * @param [Integer] node_a_id first node
 * @param [Integer] node_b_id second node
 * @return [Float] distance between identified nodes
 */
VALUE euclidean_nodes_rbobject__distance_between( VALUE self, VALUE rv_node_a_id, VALUE rv_node_b_id ) {
  int node_a_id, node_b_id;
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );

  node_a_id = NUM2INT( rv_node_a_id );
  if (node_a_id < 0 || node_a_id >= euclidean_nodes->num_nodes) {
    rb_raise(rb_eArgError, "node_a_id %d is outside accepted range 0..%d", node_a_id, euclidean_nodes->num_nodes-1);
  }

  node_b_id = NUM2INT( rv_node_b_id );
  if (node_b_id < 0 || node_b_id >= euclidean_nodes->num_nodes) {
    rb_raise(rb_eArgError, "node_b_id %d is outside accepted range 0..%d", node_b_id, euclidean_nodes->num_nodes-1);
  }

  return DBL2NUM( euclidean_nodes__distance_between( euclidean_nodes, node_a_id, node_b_id ) );
}

/* @overload all_distances_from( node_id )
 * Returns distance metric from one node to all other nodes.
 * @param [Integer] node_id node to measure from
 * @return [NArray] all distances from given node, indexed by destination node_id
 */
VALUE euclidean_nodes_rbobject__all_distances_from( VALUE self, VALUE rv_node_id ) {
  int node_id;
  VALUE rv_result;
  struct NARRAY *narr;
  int shape[1] = { 0 };
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );

  node_id = NUM2INT( rv_node_id );
  if (node_id < 0 || node_id >= euclidean_nodes->num_nodes) {
    rb_raise(rb_eArgError, "node_id %d is outside accepted range 0..%d", node_id, euclidean_nodes->num_nodes-1);
  }

  shape[0] = euclidean_nodes->num_nodes;
  rv_result = na_make_object( NA_DFLOAT, 1, shape, cNArray );
  GetNArray( rv_result, narr );
  euclidean_nodes__all_distances_from( euclidean_nodes, node_id, (double*) narr->ptr );
  return rv_result;
}

/* @overload to_weight_matrix( )
 * Converts to a weight matrix
 * @return [TspKit::Nodes::Euclidean] weight matrix representation of same problem
 */
VALUE euclidean_nodes_rbobject__to_weight_matrix( VALUE self ) {
  VALUE rv_wm;
  WeightMatrix *wm;
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );

  if ( euclidean_nodes->num_nodes > 50000 ) {
    rb_raise(rb_eRuntimeError, "number of nodes %d is too large for conversion", euclidean_nodes->num_nodes);
  }

  wm = euclidean_nodes__create_weight_matrix( euclidean_nodes );
  rv_wm = weight_matrix_as_ruby_class( wm, TspKit_WeightMatrix );

  return rv_wm;
}

/* @overload to_distance_rank( max_rank )
 * Creates a new DistanceRank from the distances
 * @param [Integer] max_rank maximum distance rank distance to consider
 * @return [TspKit::DistanceRank] distance rank summary
 */
VALUE euclidean_nodes_rbobject__to_distance_rank( VALUE self, VALUE rv_max_rank ) {
  int max_rank;
  VALUE rv_dr;
  DistanceRank *dr;
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );

  max_rank = NUM2INT(rv_max_rank);
  if ( max_rank < 2 || max_rank > (euclidean_nodes->num_nodes - 1) ) {
    rb_raise(rb_eArgError, "max_rank %d is out of bounds 2..%d", max_rank, (euclidean_nodes->num_nodes - 1));
  }

  dr = distance_rank__from_euclidean_nodes( euclidean_nodes, max_rank );
  rv_dr = distance_rank_as_ruby_class( dr, TspKit_DistanceRank );

  return rv_dr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void init_euclidean_nodes_class( ) {
  // EuclideanNodes instantiation and class methods
  rb_define_alloc_func( TspKit_EuclideanNodes, euclidean_nodes_alloc );
  rb_define_method( TspKit_EuclideanNodes, "initialize", euclidean_nodes_rbobject__initialize, 2 );
  rb_define_method( TspKit_EuclideanNodes, "initialize_copy", euclidean_nodes_rbobject__initialize_copy, 1 );
  rb_define_singleton_method( TspKit_EuclideanNodes, "from_data", euclidean_nodes_rbclass__from_data, 1 );

  // EuclideanNodes attributes
  rb_define_method( TspKit_EuclideanNodes, "num_nodes", euclidean_nodes_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_EuclideanNodes, "num_dims", euclidean_nodes_rbobject__get_num_dims, 0 );
  rb_define_method( TspKit_EuclideanNodes, "locations", euclidean_nodes_rbobject__get_narr_locations, 0 );

  // EuclideanNodes methods
  rb_define_method( TspKit_EuclideanNodes, "distance_between", euclidean_nodes_rbobject__distance_between, 2 );
  rb_define_method( TspKit_EuclideanNodes, "all_distances_from", euclidean_nodes_rbobject__all_distances_from, 1 );
  rb_define_method( TspKit_EuclideanNodes, "to_weight_matrix", euclidean_nodes_rbobject__to_weight_matrix, 0 );
  rb_define_method( TspKit_EuclideanNodes, "to_distance_rank", euclidean_nodes_rbobject__to_distance_rank, 1 );

}
