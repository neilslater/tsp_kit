// ext/tsp_kit/rub_module_tsp_kit.c

#include "ruby_module_tsp_kit.h"

/* @overload srand( seed )
 * Seed the random number generator used inside TspKit.
 * @param [Integer] seed 32-bit seed number
 * @return [nil]
 */
static VALUE rbmodule__srand( VALUE self, VALUE rv_seed ) {
  init_genrand( NUM2ULONG( rv_seed ) );
  return Qnil;
}

static unsigned long tsp_kit_srand_seed[640];

/* @overload srand_array( seed )
 * Seed the random number generator used inside TspKit.
 * @param [Array<Integer>] seed an array of up to 640 times 32 bit seed numbers
 * @return [nil]
 */
static VALUE rbmodule__srand_array( VALUE self, VALUE rv_seed_array ) {
  int i, n;
  Check_Type( rv_seed_array, T_ARRAY );
  n = FIX2INT( rb_funcall( rv_seed_array, rb_intern("count"), 0 ) );
  if ( n < 1 ) {
    rb_raise( rb_eArgError, "empty array cannot be used to seed RNG" );
  }
  if ( n > 640 ) { n = 640; }
  for ( i = 0; i < n; i++ ) {
    tsp_kit_srand_seed[i] = NUM2ULONG( rb_ary_entry( rv_seed_array, i ) );
  }
  init_by_array( tsp_kit_srand_seed, n );
  return Qnil;
}

/* @overload rand( )
 * @!visibility private
 * Use the random number generator (Ruby binding only used for tests)
 * @return [Float] random number in range 0.0..1.0
 */
static VALUE rbmodule__rand_float( VALUE self ) {
  return FLT2NUM( genrand_real1() );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VALUE TspKit = Qnil;
VALUE TspKit_EuclideanNodes = Qnil;
VALUE TspKit_WeightMatrix = Qnil;
VALUE TspKit_DistanceRank = Qnil;
VALUE TspKit_Solution = Qnil;
VALUE TspKit_GreedySolver = Qnil;
VALUE TspKit_OneTree = Qnil;
VALUE TspKit_PriorityQueue = Qnil;

void init_module_tsp_kit() {
  TspKit = rb_define_module( "TspKit" );
  TspKit_EuclideanNodes = rb_define_class_under( TspKit, "EuclideanNodes", rb_cObject );
  TspKit_WeightMatrix = rb_define_class_under( TspKit, "WeightMatrix", rb_cObject );
  TspKit_DistanceRank = rb_define_class_under( TspKit, "DistanceRank", rb_cObject );
  TspKit_Solution = rb_define_class_under( TspKit, "Solution", rb_cObject );
  TspKit_GreedySolver = rb_define_class_under( TspKit, "GreedySolver", rb_cObject );
  TspKit_OneTree = rb_define_class_under( TspKit, "OneTree", rb_cObject );
  TspKit_PriorityQueue = rb_define_class_under( TspKit, "PriorityQueue", rb_cObject );

  rb_define_singleton_method( TspKit, "srand", rbmodule__srand, 1 );
  rb_define_singleton_method( TspKit, "srand_array", rbmodule__srand_array, 1 );
  rb_define_singleton_method( TspKit, "rand", rbmodule__rand_float, 0 );

  init_euclidean_nodes_class();
  init_weight_matrix_class();
  init_distance_rank_class();
  init_solution_class();
  init_greedy_solver_class();
  init_one_tree_class();
  init_priority_queue_class();
  init_srand_by_time();
}
