// ext/tsp_kit/ruby_class_cost_matrix.c

#include "ruby_class_cost_matrix.h"
#include "ruby_class_distance_rank.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_cost_matrix.c
//

VALUE cost_matrix_as_ruby_class( CostMatrix *cost_matrix , VALUE klass ) {
  return Data_Wrap_Struct( klass, cost_matrix__gc_mark, cost_matrix__destroy, cost_matrix );
}

VALUE cost_matrix_alloc(VALUE klass) {
  return cost_matrix_as_ruby_class( cost_matrix__create(), klass );
}

CostMatrix *get_cost_matrix_struct( VALUE obj ) {
  CostMatrix *cost_matrix;
  Data_Get_Struct( obj, CostMatrix, cost_matrix );
  return cost_matrix;
}

void assert_value_wraps_cost_matrix( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)cost_matrix__destroy) {
    rb_raise( rb_eTypeError, "Expected a CostMatrix object, but got something else" );
  }
}

/* Document-class: TspKit::CostMatrix
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes )
 * Creates a new ...
 * @param [Integer] num_nodes ...
 * @return [TspKit::CostMatrix] new ...
 */
VALUE cost_matrix_rbobject__initialize( VALUE self, VALUE rv_num_nodes ) {
  int num_nodes;
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );

  num_nodes = NUM2INT( rv_num_nodes );
  if (num_nodes < 3 || num_nodes > 50000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..50000", num_nodes);
  }

  cost_matrix__init( cost_matrix, num_nodes );

  return self;
}

/* @overload clone
 * When cloned, the returned CostMatrix has deep copies of C data.
 * @return [TspKit::CostMatrix] new
 */
VALUE cost_matrix_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  CostMatrix *cost_matrix_copy;
  CostMatrix *cost_matrix_orig;

  if (copy == orig) return copy;
  cost_matrix_orig = get_cost_matrix_struct( orig );
  cost_matrix_copy = get_cost_matrix_struct( copy );

  cost_matrix__deep_copy( cost_matrix_copy, cost_matrix_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Description goes here
 * @return [Integer]
 */
VALUE cost_matrix_rbobject__get_num_nodes( VALUE self ) {
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );
  return INT2NUM( cost_matrix->num_nodes );
}

/* @!attribute [r] weights
 * Description goes here
 * @return [NArray<float>]
 */
VALUE cost_matrix_rbobject__get_narr_weights( VALUE self ) {
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );
  return cost_matrix->narr_weights;
}

/* @overload from_data( weights )
 * Creates new TspKit::Nodes::CostMatrix object directly from NArray of locations
 *
 * @return [TspKit::Nodes::CostMatrix] new instance
 */
VALUE cost_matrix_rbclass__from_data( VALUE self, VALUE rv_weights) {
  struct NARRAY *narr;
  int num_nodes;
  VALUE rv_nodes;
  CostMatrix *nodes;

  rv_weights = na_cast_object(rv_weights, NA_DFLOAT);
  GetNArray( rv_weights, narr );
  if (narr->rank != 2) {
    rb_raise(rb_eArgError, "weights array should have rank 2, but is rank %d", narr->rank);
  }

  num_nodes = narr->shape[0];
  if ( num_nodes != narr->shape[1] ) {
    rb_raise(rb_eArgError, "weights array is not square" );
  }

  if (num_nodes < 3 || num_nodes > 50000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..50000", num_nodes);
  }

  rv_nodes = cost_matrix_alloc( TspKit_CostMatrix );
  nodes = get_cost_matrix_struct( rv_nodes );
  nodes->num_nodes = num_nodes;
  nodes->narr_weights = rv_weights;
  nodes->weights = (double *) narr->ptr;
  nodes->weights_shape = ALLOC_N( int, 2 );
  nodes->weights_shape[0] = num_nodes;
  nodes->weights_shape[1] = num_nodes;

  if ( ! cost_matrix__validate( nodes ) ) {
    rb_raise( rb_eArgError, "weights array is not valid" );
  }

  return rv_nodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/* @overload validate( )
 * Returns true if TspKit::Nodes::CostMatrix object has valid distances
 *
 * @return [Boolean]
 */
VALUE cost_matrix_rbobject__validate( VALUE self ) {
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );
  if ( cost_matrix__validate( cost_matrix ) ) {
    return Qtrue;
  }
  return Qfalse;
}

/* @overload distance_between( node_a_id, node_b_id )
 * Returns distance metric between two nodes.
 * @param [Integer] node_a_id first node
 * @param [Integer] node_b_id second node
 * @return [Float] distance between identified nodes
 */
VALUE cost_matrix_rbobject__distance_between( VALUE self, VALUE rv_node_a_id, VALUE rv_node_b_id ) {
  int node_a_id, node_b_id;
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );

  node_a_id = NUM2INT( rv_node_a_id );
  if (node_a_id < 0 || node_a_id >= cost_matrix->num_nodes) {
    rb_raise(rb_eArgError, "node_a_id %d is outside accepted range 0..%d", node_a_id, cost_matrix->num_nodes-1);
  }

  node_b_id = NUM2INT( rv_node_b_id );
  if (node_b_id < 0 || node_b_id >= cost_matrix->num_nodes) {
    rb_raise(rb_eArgError, "node_b_id %d is outside accepted range 0..%d", node_b_id, cost_matrix->num_nodes-1);
  }

  return DBL2NUM( cost_matrix__distance_between( cost_matrix, node_a_id, node_b_id ) );
}

/* @overload all_distances_from( node_id )
 * Returns distance metric from one node to all other nodes.
 * @param [Integer] node_id node to measure from
 * @return [NArray] all distances from given node, indexed by destination node_id
 */
VALUE cost_matrix_rbobject__all_distances_from( VALUE self, VALUE rv_node_id ) {
  int node_id;
  VALUE rv_result;
  struct NARRAY *narr;
  int shape[1] = { 0 };
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );

  node_id = NUM2INT( rv_node_id );
  if (node_id < 0 || node_id >= cost_matrix->num_nodes) {
    rb_raise(rb_eArgError, "node_id %d is outside accepted range 0..%d", node_id, cost_matrix->num_nodes-1);
  }

  shape[0] = cost_matrix->num_nodes;
  rv_result = na_make_object( NA_DFLOAT, 1, shape, cNArray );
  GetNArray( rv_result, narr );
  cost_matrix__all_distances_from( cost_matrix, node_id, (double*) narr->ptr );
  return rv_result;
}

/* @overload to_distance_rank( max_rank )
 * Creates a new DistanceRank from the distances
 * @param [Integer] max_rank maximum distance rank distance to consider
 * @return [TspKit::DistanceRank] distance rank summary
 */
VALUE cost_matrix_rbobject__to_distance_rank( VALUE self, VALUE rv_max_rank ) {
  int max_rank;
  VALUE rv_dr;
  DistanceRank *dr;
  CostMatrix *cost_matrix = get_cost_matrix_struct( self );

  max_rank = NUM2INT(rv_max_rank);
  if ( max_rank < 2 || max_rank > (cost_matrix->num_nodes - 1) ) {
    rb_raise(rb_eArgError, "max_rank %d is out of bounds 2..%d", max_rank, (cost_matrix->num_nodes - 1));
  }

  dr = distance_rank__from_cost_matrix( cost_matrix, max_rank );
  rv_dr = distance_rank_as_ruby_class( dr, TspKit_DistanceRank );

  return rv_dr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void init_cost_matrix_class( ) {
  // CostMatrix instantiation and class methods
  rb_define_alloc_func( TspKit_CostMatrix, cost_matrix_alloc );
  rb_define_method( TspKit_CostMatrix, "initialize", cost_matrix_rbobject__initialize, 1 );
  rb_define_method( TspKit_CostMatrix, "initialize_copy", cost_matrix_rbobject__initialize_copy, 1 );
  rb_define_singleton_method( TspKit_CostMatrix, "from_data", cost_matrix_rbclass__from_data, 1 );

  // CostMatrix attributes
  rb_define_method( TspKit_CostMatrix, "num_nodes", cost_matrix_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_CostMatrix, "weights", cost_matrix_rbobject__get_narr_weights, 0 );

  // CostMatrix methods
  rb_define_method( TspKit_CostMatrix, "validate", cost_matrix_rbobject__validate, 0 );
  rb_define_method( TspKit_CostMatrix, "distance_between", cost_matrix_rbobject__distance_between, 2 );
  rb_define_method( TspKit_CostMatrix, "all_distances_from", cost_matrix_rbobject__all_distances_from, 1 );
  rb_define_method( TspKit_CostMatrix, "to_distance_rank", cost_matrix_rbobject__to_distance_rank, 1 );
}
