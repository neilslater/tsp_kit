// ext/tsp_kit/ruby_class_one_tree.c

#include "ruby_class_one_tree.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_one_tree.c
//

inline VALUE one_tree_as_ruby_class( OneTree *one_tree , VALUE klass ) {
  return Data_Wrap_Struct( klass, one_tree__gc_mark, one_tree__destroy, one_tree );
}

VALUE one_tree_alloc(VALUE klass) {
  return one_tree_as_ruby_class( one_tree__create(), klass );
}

inline OneTree *get_one_tree_struct( VALUE obj ) {
  OneTree *one_tree;
  Data_Get_Struct( obj, OneTree, one_tree );
  return one_tree;
}

void assert_value_wraps_one_tree( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)one_tree__destroy) {
    rb_raise( rb_eTypeError, "Expected a OneTree object, but got something else" );
  }
}

/* Document-class: TspKit::OneTree
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes )
 * Creates a new ...
 * @param [Integer] num_nodes ...
 * @return [TspKit::OneTree] new ...
 */
VALUE one_tree_rbobject__initialize( VALUE self, VALUE rv_num_nodes ) {
  OneTree *one_tree = get_one_tree_struct( self );

  one_tree__init( one_tree, NUM2INT( rv_num_nodes ) );

  return self;
}

/* @overload clone
 * When cloned, the returned OneTree has deep copies of C data.
 * @return [TspKit::OneTree] new
 */
VALUE one_tree_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  OneTree *one_tree_copy;
  OneTree *one_tree_orig;

  if (copy == orig) return copy;
  one_tree_orig = get_one_tree_struct( orig );
  one_tree_copy = get_one_tree_struct( copy );

  one_tree__deep_copy( one_tree_copy, one_tree_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Description goes here
 * @return [Integer]
 */
VALUE one_tree_rbobject__get_num_nodes( VALUE self ) {
  OneTree *one_tree = get_one_tree_struct( self );
  return INT2NUM( one_tree->num_nodes );
}

/* @!attribute [r] node_penalties
 * Description goes here
 * @return [NArray<float>]
 */
VALUE one_tree_rbobject__get_narr_node_penalties( VALUE self ) {
  OneTree *one_tree = get_one_tree_struct( self );
  return one_tree->narr_node_penalties;
}

/* @!attribute [r] node_ids
 * Description goes here
 * @return [NArray<int>]
 */
VALUE one_tree_rbobject__get_narr_node_ids( VALUE self ) {
  OneTree *one_tree = get_one_tree_struct( self );
  return one_tree->narr_node_ids;
}

/* @!attribute [r] parents
 * Description goes here
 * @return [NArray<int>]
 */
VALUE one_tree_rbobject__get_narr_parents( VALUE self ) {
  OneTree *one_tree = get_one_tree_struct( self );
  return one_tree->narr_parents;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_one_tree_class( ) {
  // OneTree instantiation and class methods
  rb_define_alloc_func( TspKit_OneTree, one_tree_alloc );
  rb_define_method( TspKit_OneTree, "initialize", one_tree_rbobject__initialize, 1 );
  rb_define_method( TspKit_OneTree, "initialize_copy", one_tree_rbobject__initialize_copy, 1 );

  // OneTree attributes
  rb_define_method( TspKit_OneTree, "num_nodes", one_tree_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_OneTree, "node_penalties", one_tree_rbobject__get_narr_node_penalties, 0 );
  rb_define_method( TspKit_OneTree, "node_ids", one_tree_rbobject__get_narr_node_ids, 0 );
  rb_define_method( TspKit_OneTree, "parents", one_tree_rbobject__get_narr_parents, 0 );
}
