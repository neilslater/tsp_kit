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
  WeightMatrix *weight_matrix = get_weight_matrix_struct( self );

  weight_matrix__init( weight_matrix, NUM2INT( rv_num_nodes ) );

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

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_weight_matrix_class( ) {
  // WeightMatrix instantiation and class methods
  rb_define_alloc_func( TspKit_WeightMatrix, weight_matrix_alloc );
  rb_define_method( TspKit_WeightMatrix, "initialize", weight_matrix_rbobject__initialize, 1 );
  rb_define_method( TspKit_WeightMatrix, "initialize_copy", weight_matrix_rbobject__initialize_copy, 1 );

  // WeightMatrix attributes
  rb_define_method( TspKit_WeightMatrix, "num_nodes", weight_matrix_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_WeightMatrix, "weights", weight_matrix_rbobject__get_narr_weights, 0 );
}
