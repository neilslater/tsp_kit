// ext/tsp_kit/rub_module_tsp_kit.c

#include "ruby_module_tsp_kit.h"
#include "utilities.h"

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

/* @overload randn( )
 * @!visibility private
 * Use the random number generator (Ruby binding only used for tests)
 * @return [Float] random number in range 0.0..1.0
 */
static VALUE rbmodule__randn_float( VALUE self ) {
  return FLT2NUM( genrand_norm() );
}

/* @overload shuffle_narray( narray )
 * @!visibility private
 * Use the random number generator to shuffle some integers in a NArray
 * @return [NArray] original NArray
 */
static VALUE rbmodule__shuffle_narray( VALUE self, VALUE rv_narray ) {
  struct NARRAY *narr;
  GetNArray( rv_narray, narr );

  // TODO: Raise error if NArray is wrong type

  shuffle_ints( narr->total, (int *) narr->ptr );
  return rv_narray;
}

/* @overload quicksort_a_by_b( narray_a, narray_b )
 * @!visibility private
 * Test quicksort_by
 * @return [NArray] original NArray
 */
static VALUE rbmodule__quicksort_a_by_b( VALUE self, VALUE rv_narray_a, VALUE rv_narray_b ) {
  struct NARRAY *narr_a;
  struct NARRAY *narr_b;

  GetNArray( rv_narray_a, narr_a );
  GetNArray( rv_narray_b, narr_b );

  // TODO: Raise error if either NArray is wrong type or different size, or if a has ids outside of b
  if ( narr_b->total != narr_a->total ) {
    rb_raise( rb_eArgError, "arrays must be same size" );
  }
  quicksort_ids_by_double( (int *) narr_a->ptr, (double *) narr_b->ptr, 0, narr_b->total - 1 );

  return rv_narray_a;
}

/* @overload quicksort_ints( narray )
 * @!visibility private
 * Test quicksort
 * @return [NArray] original NArray
 */
static VALUE rbmodule__quicksort_ints( VALUE self, VALUE rv_narray ) {
  struct NARRAY *narr;

  GetNArray( rv_narray, narr );

  // TODO: Raise error if NArray is wrong type
  quicksort_ints( (int *) narr->ptr, 0, narr->total - 1 );

  return rv_narray;
}

/* @overload halfnorm_int( stdev )
 * Description goes here
 * @return [Integer]
 */
VALUE rbmodule__halfnorm_int( VALUE self, VALUE rv_stdev ) {
  float s = NUM2FLT(rv_stdev);
  return INT2NUM(half_norm_int(s));
}

/* @overload random_int_up_to( max_int )
 * Description goes here
 * @return [Integer]
 */
VALUE rbmodule__random_int_up_to( VALUE self, VALUE rv_n ) {
  int n = NUM2INT(rv_n);
  return INT2NUM(random_int_up_to(n));
}

////////////////////////////////////////////////////////////////////////////////////////////////////

VALUE TspKit = Qnil;

VALUE TspKit_Nodes = Qnil;
VALUE TspKit_EuclideanNodes = Qnil;
VALUE TspKit_WeightMatrix = Qnil;

VALUE TspKit_DistanceRank = Qnil;

VALUE TspKit_Utils = Qnil;
VALUE TspKit_PriorityQueue = Qnil;

VALUE TspKit_Solution = Qnil;
VALUE TspKit_GreedySolver = Qnil;
VALUE TspKit_OneTree = Qnil;

void init_module_tsp_kit() {
  TspKit = rb_define_module( "TspKit" );

  // Node types
  TspKit_Nodes = rb_define_class_under( TspKit, "Nodes", rb_cObject );
  TspKit_EuclideanNodes = rb_define_class_under( TspKit_Nodes, "Euclidean", rb_cObject );
  TspKit_WeightMatrix = rb_define_class_under( TspKit_Nodes, "WeightMatrix", rb_cObject );

  // Utilities
  TspKit_Utils = rb_define_class_under( TspKit, "Utils", rb_cObject );
  TspKit_PriorityQueue = rb_define_class_under( TspKit_Utils, "PriorityQueue", rb_cObject );
  TspKit_OneTree = rb_define_class_under( TspKit_Utils, "OneTree", rb_cObject );

  TspKit_DistanceRank = rb_define_class_under( TspKit, "DistanceRank", rb_cObject );
  TspKit_Solution = rb_define_class_under( TspKit, "Solution", rb_cObject );

  TspKit_GreedySolver = rb_define_class_under( TspKit, "GreedySolver", rb_cObject );

  rb_define_singleton_method( TspKit, "srand", rbmodule__srand, 1 );
  rb_define_singleton_method( TspKit, "srand_array", rbmodule__srand_array, 1 );
  rb_define_singleton_method( TspKit, "rand", rbmodule__rand_float, 0 );
  rb_define_singleton_method( TspKit, "randn", rbmodule__randn_float, 0 );
  rb_define_singleton_method( TspKit, "halfnorm_int", rbmodule__halfnorm_int, 1 );
  rb_define_singleton_method( TspKit, "random_int_up_to", rbmodule__random_int_up_to, 1 );
  rb_define_singleton_method( TspKit, "shuffle_narray", rbmodule__shuffle_narray, 1 );
  rb_define_singleton_method( TspKit, "quicksort_a_by_b", rbmodule__quicksort_a_by_b, 2 );
  rb_define_singleton_method( TspKit, "quicksort_ints", rbmodule__quicksort_ints, 1 );

  init_euclidean_nodes_class();
  init_weight_matrix_class();
  init_distance_rank_class();
  init_solution_class();
  init_greedy_solver_class();
  init_one_tree_class();
  init_priority_queue_class();
  init_srand_by_time();
}
