// ext/tsp_kit/ruby_class_weight_matrix.c

#include "ruby_class_weight_matrix.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_weight_matrix.c
//

inline VALUE weight_matrix_as_ruby_class( WeightMatrix *weight_matrix , VALUE klass ) {
  return Data_Wrap_Struct( klass, weight_matrix__gc_mark, weight_matrix__destroy, weight_matrix );
}

VALUE weight_matrix_alloc(VALUE klass) {
  return weight_matrix_as_ruby_class( weight_matrix__create(), klass );
}

inline WeightMatrix *get_weight_matrix_struct( VALUE obj ) {
  WeightMatrix *weight_matrix;
  Data_Get_Struct( obj, WeightMatrix, weight_matrix );
  return weight_matrix;
}

void assert_value_wraps_weight_matrix( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)weight_matrix__destroy) {
    rb_raise( rb_eTypeError, "Expected a WeightMatrix object, but got something else" );
  }
}

/* Document-class: TspKit::WeightMatrix
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes )
 * Creates a new ...
 * @param [Integer] num_nodes ...
 * @return [TspKit::WeightMatrix] new ...
 */
VALUE weight_matrix_rbobject__initialize( VALUE self, VALUE rv_num_nodes ) {
  int num_nodes;
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );

  num_nodes = NUM2INT( rv_num_nodes );
  if (num_nodes < 3 || num_nodes > 50000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..50000", num_nodes);
  }

  weight_matrix__init( weight_matrix, num_nodes );

  return self;
}

/* @overload clone
 * When cloned, the returned WeightMatrix has deep copies of C data.
 * @return [TspKit::WeightMatrix] new
 */
VALUE weight_matrix_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  WeightMatrix *weight_matrix_copy;
  WeightMatrix *weight_matrix_orig;

  if (copy == orig) return copy;
  weight_matrix_orig = get_weight_matrix_struct( orig );
  weight_matrix_copy = get_weight_matrix_struct( copy );

  weight_matrix__deep_copy( weight_matrix_copy, weight_matrix_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Description goes here
 * @return [Integer]
 */
VALUE weight_matrix_rbobject__get_num_nodes( VALUE self ) {
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );
  return INT2NUM( weight_matrix->num_nodes );
}

/* @!attribute [r] weights
 * Description goes here
 * @return [NArray<float>]
 */
VALUE weight_matrix_rbobject__get_narr_weights( VALUE self ) {
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );
  return weight_matrix->narr_weights;
}

/* @overload from_data( weights )
 * Creates new TspKit::Nodes::WeightMatrix object directly from NArray of locations
 *
 * @return [TspKit::Nodes::WeightMatrix] new instance
 */
VALUE weight_matrix_rbclass__from_data( VALUE self, VALUE rv_weights) {
  struct NARRAY *narr;
  int num_nodes;
  VALUE rv_nodes;
  WeightMatrix *nodes;

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

  rv_nodes = weight_matrix_alloc( TspKit_WeightMatrix );
  nodes = get_weight_matrix_struct( rv_nodes );
  nodes->num_nodes = num_nodes;
  nodes->narr_weights = rv_weights;
  nodes->weights = (double *) narr->ptr;
  nodes->weights_shape = ALLOC_N( int, 2 );
  nodes->weights_shape[0] = num_nodes;
  nodes->weights_shape[1] = num_nodes;

  if ( ! weight_matrix__validate( nodes ) ) {
    rb_raise( rb_eArgError, "weights array is not valid" );
  }

  return rv_nodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/* @overload validate( )
 * Returns true if TspKit::Nodes::WeightMatrix object has valid distances
 *
 * @return [Boolean]
 */
VALUE weight_matrix_rbobject__validate( VALUE self ) {
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );
  if ( weight_matrix__validate( weight_matrix ) ) {
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
VALUE weight_matrix_rbobject__distance_between( VALUE self, VALUE rv_node_a_id, VALUE rv_node_b_id ) {
  int node_a_id, node_b_id;
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );

  node_a_id = NUM2INT( rv_node_a_id );
  if (node_a_id < 0 || node_a_id >= weight_matrix->num_nodes) {
    rb_raise(rb_eArgError, "node_a_id %d is outside accepted range 0..%d", node_a_id, weight_matrix->num_nodes-1);
  }

  node_b_id = NUM2INT( rv_node_b_id );
  if (node_b_id < 0 || node_b_id >= weight_matrix->num_nodes) {
    rb_raise(rb_eArgError, "node_b_id %d is outside accepted range 0..%d", node_b_id, weight_matrix->num_nodes-1);
  }

  return DBL2NUM( weight_matrix__distance_between( weight_matrix, node_a_id, node_b_id ) );
}

/* @overload all_distances_from( node_id )
 * Returns distance metric from one node to all other nodes.
 * @param [Integer] node_id node to measure from
 * @return [NArray] all distances from given node, indexed by destination node_id
 */
VALUE weight_matrix_rbobject__all_distances_from( VALUE self, VALUE rv_node_id ) {
  int node_id;
  VALUE rv_result;
  struct NARRAY *narr;
  int shape[1] = { 0 };
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );

  node_id = NUM2INT( rv_node_id );
  if (node_id < 0 || node_id >= weight_matrix->num_nodes) {
    rb_raise(rb_eArgError, "node_id %d is outside accepted range 0..%d", node_id, weight_matrix->num_nodes-1);
  }

  shape[0] = weight_matrix->num_nodes;
  rv_result = na_make_object( NA_DFLOAT, 1, shape, cNArray );
  GetNArray( rv_result, narr );
  weight_matrix__all_distances_from( weight_matrix, node_id, (double*) narr->ptr );
  return rv_result;
}

void init_weight_matrix_class( ) {
  // WeightMatrix instantiation and class methods
  rb_define_alloc_func( TspKit_WeightMatrix, weight_matrix_alloc );
  rb_define_method( TspKit_WeightMatrix, "initialize", weight_matrix_rbobject__initialize, 1 );
  rb_define_method( TspKit_WeightMatrix, "initialize_copy", weight_matrix_rbobject__initialize_copy, 1 );
  rb_define_singleton_method( TspKit_WeightMatrix, "from_data", weight_matrix_rbclass__from_data, 1 );

  // WeightMatrix attributes
  rb_define_method( TspKit_WeightMatrix, "num_nodes", weight_matrix_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_WeightMatrix, "weights", weight_matrix_rbobject__get_narr_weights, 0 );

  // WeightMatrix methods
  rb_define_method( TspKit_WeightMatrix, "validate", weight_matrix_rbobject__validate, 0 );
  rb_define_method( TspKit_WeightMatrix, "distance_between", weight_matrix_rbobject__distance_between, 2 );
  rb_define_method( TspKit_WeightMatrix, "all_distances_from", weight_matrix_rbobject__all_distances_from, 1 );
}
