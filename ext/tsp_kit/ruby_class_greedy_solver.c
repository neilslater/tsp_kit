// ext/tsp_kit/ruby_class_greedy_solver.c

#include "ruby_class_greedy_solver.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_greedy_solver.c
//

VALUE greedy_solver_as_ruby_class( GreedySolver *greedy_solver , VALUE klass ) {
  return Data_Wrap_Struct( klass, greedy_solver__gc_mark, greedy_solver__destroy, greedy_solver );
}

VALUE greedy_solver_alloc(VALUE klass) {
  return greedy_solver_as_ruby_class( greedy_solver__create(), klass );
}

GreedySolver *get_greedy_solver_struct( VALUE obj ) {
  GreedySolver *greedy_solver;
  Data_Get_Struct( obj, GreedySolver, greedy_solver );
  return greedy_solver;
}

void assert_value_wraps_greedy_solver( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)greedy_solver__destroy) {
    rb_raise( rb_eTypeError, "Expected a GreedySolver object, but got something else" );
  }
}

/* Document-class: TspKit::GreedySolver
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes )
 * Creates a new ...
 * @param [Integer] num_nodes ...
 * @return [TspKit::GreedySolver] new ...
 */
VALUE greedy_solver_rbobject__initialize( VALUE self, VALUE rv_num_nodes ) {
  GreedySolver *greedy_solver = get_greedy_solver_struct( self );

  greedy_solver__init( greedy_solver, NUM2INT( rv_num_nodes ) );

  return self;
}

/* @overload clone
 * When cloned, the returned GreedySolver has deep copies of C data.
 * @return [TspKit::GreedySolver] new
 */
VALUE greedy_solver_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  GreedySolver *greedy_solver_copy;
  GreedySolver *greedy_solver_orig;

  if (copy == orig) return copy;
  greedy_solver_orig = get_greedy_solver_struct( orig );
  greedy_solver_copy = get_greedy_solver_struct( copy );

  greedy_solver__deep_copy( greedy_solver_copy, greedy_solver_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Description goes here
 * @return [Integer]
 */
VALUE greedy_solver_rbobject__get_num_nodes( VALUE self ) {
  GreedySolver *greedy_solver = get_greedy_solver_struct( self );
  return INT2NUM( greedy_solver->num_nodes );
}

/* @!attribute [r] max_section_id
 * Description goes here
 * @return [Integer]
 */
VALUE greedy_solver_rbobject__get_max_section_id( VALUE self ) {
  GreedySolver *greedy_solver = get_greedy_solver_struct( self );
  return INT2NUM( greedy_solver->max_section_id );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_greedy_solver_class( ) {
  // GreedySolver instantiation and class methods
  rb_define_alloc_func( TspKit_GreedySolver, greedy_solver_alloc );
  rb_define_method( TspKit_GreedySolver, "initialize", greedy_solver_rbobject__initialize, 1 );
  rb_define_method( TspKit_GreedySolver, "initialize_copy", greedy_solver_rbobject__initialize_copy, 1 );

  // GreedySolver attributes
  rb_define_method( TspKit_GreedySolver, "num_nodes", greedy_solver_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_GreedySolver, "max_section_id", greedy_solver_rbobject__get_max_section_id, 0 );
}
