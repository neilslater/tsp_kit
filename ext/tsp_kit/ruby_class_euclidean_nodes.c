// ext/tsp_kit/ruby_class_euclidean_nodes.c

#include "ruby_class_euclidean_nodes.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_euclidean_nodes.c
//

inline VALUE euclidean_nodes_as_ruby_class( EuclideanNodes *euclidean_nodes , VALUE klass ) {
  return Data_Wrap_Struct( klass, euclidean_nodes__gc_mark, euclidean_nodes__destroy, euclidean_nodes );
}

VALUE euclidean_nodes_alloc(VALUE klass) {
  return euclidean_nodes_as_ruby_class( euclidean_nodes__create(), klass );
}

inline EuclideanNodes *get_euclidean_nodes_struct( VALUE obj ) {
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
 * Creates a new ...
 * @param [Integer] num_nodes ...
 * @param [Integer] num_dims ...
 * @return [TspKit::EuclideanNodes] new ...
 */
VALUE euclidean_nodes_rbobject__initialize( VALUE self, VALUE rv_num_nodes, VALUE rv_num_dims ) {
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );

  euclidean_nodes__init( euclidean_nodes, NUM2INT( rv_num_nodes ), NUM2INT( rv_num_dims ) );

  return self;
}

/* @overload clone
 * When cloned, the returned EuclideanNodes has deep copies of C data.
 * @return [TspKit::EuclideanNodes] new
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
 * Description goes here
 * @return [Integer]
 */
VALUE euclidean_nodes_rbobject__get_num_nodes( VALUE self ) {
  EuclideanNodes *euclidean_nodes = get_euclidean_nodes_struct( self );
  return INT2NUM( euclidean_nodes->num_nodes );
}

/* @!attribute [r] num_dims
 * Description goes here
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

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_euclidean_nodes_class( ) {
  // EuclideanNodes instantiation and class methods
  rb_define_alloc_func( TspKit_EuclideanNodes, euclidean_nodes_alloc );
  rb_define_method( TspKit_EuclideanNodes, "initialize", euclidean_nodes_rbobject__initialize, 2 );
  rb_define_method( TspKit_EuclideanNodes, "initialize_copy", euclidean_nodes_rbobject__initialize_copy, 1 );

  // EuclideanNodes attributes
  rb_define_method( TspKit_EuclideanNodes, "num_nodes", euclidean_nodes_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_EuclideanNodes, "num_dims", euclidean_nodes_rbobject__get_num_dims, 0 );
  rb_define_method( TspKit_EuclideanNodes, "locations", euclidean_nodes_rbobject__get_narr_locations, 0 );
}
