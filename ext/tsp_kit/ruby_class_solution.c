// ext/tsp_kit/ruby_class_solution.c

#include "ruby_class_solution.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_solution.c
//

static const rb_data_type_t solution_data_type = {
  "TspKit::Solution",
  { (RUBY_DATA_FUNC)solution__gc_mark, (RUBY_DATA_FUNC)solution__destroy, NULL,
    (RUBY_DATA_FUNC)solution__gc_compact, { NULL } },
  NULL, NULL, RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE solution_as_ruby_class( Solution *solution , VALUE klass ) {
  return TypedData_Wrap_Struct( klass, &solution_data_type, solution );
}

VALUE solution_alloc(VALUE klass) {
  return solution_as_ruby_class( solution__create(), klass );
}

Solution *get_solution_struct( VALUE obj ) {
  Solution *solution;
  TypedData_Get_Struct( obj, Solution, &solution_data_type, solution );
  return solution;
}

void assert_value_wraps_solution( VALUE obj ) {
  if (!rb_typeddata_is_kind_of(obj, &solution_data_type)) {
    rb_raise( rb_eTypeError, "Expected a TspKit::Solution object, but got something else" );
  }
}

/* Document-class: TspKit::Solution
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes )
 * Creates a new ...
 * @param [Integer] num_nodes ...
 * @return [TspKit::Solution] new ...
 */
VALUE solution_rbobject__initialize( VALUE self, VALUE rv_num_nodes ) {
  Solution *solution = get_solution_struct( self );

  solution__init( solution, NUM2INT( rv_num_nodes ) );

  return self;
}

/* @overload clone
 * When cloned, the returned Solution has deep copies of C data.
 * @return [TspKit::Solution] new
 */
VALUE solution_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  Solution *solution_copy;
  Solution *solution_orig;

  if (copy == orig) return copy;
  solution_orig = get_solution_struct( orig );
  solution_copy = get_solution_struct( copy );

  solution__deep_copy( solution_copy, solution_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Description goes here
 * @return [Integer]
 */
VALUE solution_rbobject__get_num_nodes( VALUE self ) {
  Solution *solution = get_solution_struct( self );
  return INT2NUM( solution->num_nodes );
}

/* @!attribute [r] ids
 * Description goes here
 * @return [Numo::Int32]
 */
VALUE solution_rbobject__get_narr_ids( VALUE self ) {
  Solution *solution = get_solution_struct( self );
  return solution->narr_ids;
}

/* @!attribute [r] node_idx
 * Description goes here
 * @return [Numo::Int32]
 */
VALUE solution_rbobject__get_narr_node_idx( VALUE self ) {
  Solution *solution = get_solution_struct( self );
  return solution->narr_node_idx;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_solution_class( ) {
  // Solution instantiation and class methods
  rb_define_alloc_func( TspKit_Solution, solution_alloc );
  rb_define_method( TspKit_Solution, "initialize", solution_rbobject__initialize, 1 );
  rb_define_method( TspKit_Solution, "initialize_copy", solution_rbobject__initialize_copy, 1 );

  // Solution attributes
  rb_define_method( TspKit_Solution, "num_nodes", solution_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_Solution, "ids", solution_rbobject__get_narr_ids, 0 );
  rb_define_method( TspKit_Solution, "node_idx", solution_rbobject__get_narr_node_idx, 0 );
}
