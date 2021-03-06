// ext/tsp_kit/ruby_class_distance_rank.c

#include "ruby_class_distance_rank.h"
#include "ruby_class_euclidean_nodes.h"
#include "ruby_class_cost_matrix.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_distance_rank.c
//

VALUE distance_rank_as_ruby_class( DistanceRank *distance_rank , VALUE klass ) {
  return Data_Wrap_Struct( klass, distance_rank__gc_mark, distance_rank__destroy, distance_rank );
}

VALUE distance_rank_alloc(VALUE klass) {
  return distance_rank_as_ruby_class( distance_rank__create(), klass );
}

DistanceRank *get_distance_rank_struct( VALUE obj ) {
  DistanceRank *distance_rank;
  Data_Get_Struct( obj, DistanceRank, distance_rank );
  return distance_rank;
}

void assert_value_wraps_distance_rank( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)distance_rank__destroy) {
    rb_raise( rb_eTypeError, "Expected a DistanceRank object, but got something else" );
  }
}

/* Document-class: TspKit::DistanceRank
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( num_nodes, max_rank )
 * Creates a new TspKit::DistanceRank
 * @param [Integer] num_nodes ...
 * @param [Integer] max_rank ...
 * @return [TspKit::DistanceRank] new TspKit::DistanceRank
 */
VALUE distance_rank_rbobject__initialize( VALUE self, VALUE rv_num_nodes, VALUE rv_max_rank ) {
  DistanceRank *distance_rank = get_distance_rank_struct( self );
  int num_nodes, max_rank;

  num_nodes = NUM2INT( rv_num_nodes );
  if (num_nodes < 3 || num_nodes > 10000000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..10000000", num_nodes);
  }

  max_rank = NUM2INT( rv_max_rank );
  if ( max_rank < 2 || max_rank > (num_nodes - 1) ) {
    rb_raise(rb_eArgError, "max_rank %d is outside accepted range 2..%d", max_rank, num_nodes - 1);
  }

  distance_rank__init( distance_rank, NUM2INT( rv_num_nodes ), NUM2INT( rv_max_rank ) );

  return self;
}

/* @overload clone
 * When cloned, the returned DistanceRank has deep copies of C data.
 * @return [TspKit::DistanceRank] new
 */
VALUE distance_rank_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  DistanceRank *distance_rank_copy;
  DistanceRank *distance_rank_orig;

  if (copy == orig) return copy;
  distance_rank_orig = get_distance_rank_struct( orig );
  distance_rank_copy = get_distance_rank_struct( copy );

  distance_rank__deep_copy( distance_rank_copy, distance_rank_orig );

  return copy;
}

/* @!attribute [r] num_nodes
 * Description goes here
 * @return [Integer]
 */
VALUE distance_rank_rbobject__get_num_nodes( VALUE self ) {
  DistanceRank *distance_rank = get_distance_rank_struct( self );
  return INT2NUM( distance_rank->num_nodes );
}

/* @!attribute [r] max_rank
 * Description goes here
 * @return [Integer]
 */
VALUE distance_rank_rbobject__get_max_rank( VALUE self ) {
  DistanceRank *distance_rank = get_distance_rank_struct( self );
  return INT2NUM( distance_rank->max_rank );
}

/* @!attribute [r] closest_nodes
 * Description goes here
 * @return [NArray<int>]
 */
VALUE distance_rank_rbobject__get_narr_closest_nodes( VALUE self ) {
  DistanceRank *distance_rank = get_distance_rank_struct( self );
  return distance_rank->narr_closest_nodes;
}

////////////////////////////////////////////////////////////////////////////////////////////////////


/* @overload from_data( closest_nodes )
 * Creates new TspKit::DistanceRank object directly from NArray of closest_nodes
 *
 * @return [TspKit::DistanceRank] new instance
 */
VALUE distance_rank_rbclass__from_data( VALUE self, VALUE rv_closest_nodes) {
  struct NARRAY *narr;
  int num_nodes, max_rank;
  VALUE rv_dr;
  DistanceRank *dr;

  rv_closest_nodes = na_cast_object(rv_closest_nodes, NA_LINT);
  GetNArray( rv_closest_nodes, narr );
  if (narr->rank != 2) {
    rb_raise(rb_eArgError, "closest_nodes array should have rank 2, but is rank %d", narr->rank);
  }

  max_rank = narr->shape[0];
  num_nodes = narr->shape[1];

  if (num_nodes < 3 || num_nodes > 10000000) {
    rb_raise(rb_eArgError, "num_nodes %d is outside accepted range 3..10000000", num_nodes);
  }

  if (max_rank < 2 || max_rank > (num_nodes-1)) {
    rb_raise(rb_eArgError, "max_rank %d is outside accepted range 2..%d", max_rank, num_nodes-1);
  }

  // Possible TODO - check that contents of distance_rank are valid

  rv_dr = distance_rank_alloc( TspKit_DistanceRank );
  dr = get_distance_rank_struct( rv_dr );
  dr->num_nodes = num_nodes;
  dr->max_rank = max_rank;
  dr->narr_closest_nodes = rv_closest_nodes;
  dr->closest_nodes = (int *) narr->ptr;
  dr->closest_nodes_shape = ALLOC_N( int, 2 );
  dr->closest_nodes_shape[0] = max_rank;
  dr->closest_nodes_shape[1] = num_nodes;
  return rv_dr;
}

/* @overload resize( new_max_rank )
 * Re-sizes distance rank to new max rank. When increasing size, it does not recalculate extra distances
 * so the main use of this is to reduce the max_rank
 *
 * @return [TspKit::DistanceRank] self
 */
VALUE distance_rank_rbobject__resize( VALUE self, VALUE rv_new_max_rank ) {
  DistanceRank *distance_rank = get_distance_rank_struct( self );
  int new_max_rank = NUM2INT(rv_new_max_rank);

  if (new_max_rank < 2 || new_max_rank > (distance_rank->num_nodes - 1)) {
    rb_raise(rb_eArgError, "new_max_rank %d is outside accepted range 2..%d", new_max_rank, distance_rank->num_nodes-1);
  }

  distance_rank__resize( distance_rank, new_max_rank );

  return self;
}


/* @overload bidirectional( nodes, rank_cutoff )
 * Ajusts distance rank to cover bidirectional links. If node A has node B in the adjacency list, then
 * B will have A in its list
 *
 * @return [TspKit::DistanceRank] self
 */
VALUE distance_rank_rbobject__bidirectional( VALUE self, VALUE rv_nodes, VALUE rv_rank_cutoff ) {
  DistanceRank *distance_rank = get_distance_rank_struct( self );
  EuclideanNodes * euc_nodes;
  CostMatrix * cm_nodes;
  NodeType nodes_type = get_node_type_from_rv( rv_nodes );

  switch(nodes_type) {
    case NODE_EUC:
      euc_nodes = get_euclidean_nodes_struct( rv_nodes );
      break;
    case NODE_CM:
      cm_nodes = get_cost_matrix_struct( rv_nodes );
      break;
  }

  int rank_cutoff = NUM2INT(rv_rank_cutoff);
  if (rank_cutoff < 2 || rank_cutoff > (distance_rank->num_nodes - 1)) {
    rb_raise(rb_eArgError, "rank_cutoff %d is outside accepted range 2..%d", rank_cutoff, distance_rank->num_nodes-1);
  }

  switch(nodes_type) {
    case NODE_EUC:
      distance_rank__bidirectional( distance_rank, euc_nodes, euclidean_nodes__distance_between, rank_cutoff );
      break;
    case NODE_CM:
      distance_rank__bidirectional( distance_rank, cm_nodes, cost_matrix__distance_between, rank_cutoff );
      break;
  }

  return self;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_distance_rank_class( ) {
  // DistanceRank instantiation and class methods
  rb_define_alloc_func( TspKit_DistanceRank, distance_rank_alloc );
  rb_define_method( TspKit_DistanceRank, "initialize", distance_rank_rbobject__initialize, 2 );
  rb_define_method( TspKit_DistanceRank, "initialize_copy", distance_rank_rbobject__initialize_copy, 1 );
  rb_define_singleton_method( TspKit_DistanceRank, "from_data", distance_rank_rbclass__from_data, 1 );

  // DistanceRank attributes
  rb_define_method( TspKit_DistanceRank, "num_nodes", distance_rank_rbobject__get_num_nodes, 0 );
  rb_define_method( TspKit_DistanceRank, "max_rank", distance_rank_rbobject__get_max_rank, 0 );
  rb_define_method( TspKit_DistanceRank, "closest_nodes", distance_rank_rbobject__get_narr_closest_nodes, 0 );
  rb_define_method( TspKit_DistanceRank, "resize", distance_rank_rbobject__resize, 1 );

  rb_define_method( TspKit_DistanceRank, "bidirectional", distance_rank_rbobject__bidirectional, 2 );
}
