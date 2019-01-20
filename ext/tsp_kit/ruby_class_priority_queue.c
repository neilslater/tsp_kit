// ext/tsp_kit/ruby_class_priority_queue.c

#include "ruby_class_priority_queue.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for training data arrays - the deeper implementation is in
//  struct_priority_queue.c
//

inline VALUE priority_queue_as_ruby_class( PriorityQueue *priority_queue , VALUE klass ) {
  return Data_Wrap_Struct( klass, priority_queue__gc_mark, priority_queue__destroy, priority_queue );
}

VALUE priority_queue_alloc(VALUE klass) {
  return priority_queue_as_ruby_class( priority_queue__create(), klass );
}

inline PriorityQueue *get_priority_queue_struct( VALUE obj ) {
  PriorityQueue *priority_queue;
  Data_Get_Struct( obj, PriorityQueue, priority_queue );
  return priority_queue;
}

void assert_value_wraps_priority_queue( VALUE obj ) {
  if ( TYPE(obj) != T_DATA ||
      RDATA(obj)->dfree != (RUBY_DATA_FUNC)priority_queue__destroy) {
    rb_raise( rb_eTypeError, "Expected a PriorityQueue object, but got something else" );
  }
}

/* Document-class: TspKit::PriorityQueue
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Network method definitions
//

/* @overload initialize( pq_size )
 * Creates a new ...
 * @param [Integer] pq_size ...
 * @return [TspKit::PriorityQueue] new ...
 */
VALUE priority_queue_rbobject__initialize( VALUE self, VALUE rv_pq_size ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );

  priority_queue__init( priority_queue, NUM2INT( rv_pq_size ) );

  return self;
}

/* @overload clone
 * When cloned, the returned PriorityQueue has deep copies of C data.
 * @return [TspKit::PriorityQueue] new
 */
VALUE priority_queue_rbobject__initialize_copy( VALUE copy, VALUE orig ) {
  PriorityQueue *priority_queue_copy;
  PriorityQueue *priority_queue_orig;

  if (copy == orig) return copy;
  priority_queue_orig = get_priority_queue_struct( orig );
  priority_queue_copy = get_priority_queue_struct( copy );

  priority_queue__deep_copy( priority_queue_copy, priority_queue_orig );

  return copy;
}

/* @!attribute [r] pq_size
 * Description goes here
 * @return [Integer]
 */
VALUE priority_queue_rbobject__get_pq_size( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue->pq_size );
}

/* @!attribute [r] heap_root
 * Description goes here
 * @return [Integer]
 */
VALUE priority_queue_rbobject__get_heap_root( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue->heap_root );
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void init_priority_queue_class( ) {
  // PriorityQueue instantiation and class methods
  rb_define_alloc_func( TspKit_PriorityQueue, priority_queue_alloc );
  rb_define_method( TspKit_PriorityQueue, "initialize", priority_queue_rbobject__initialize, 1 );
  rb_define_method( TspKit_PriorityQueue, "initialize_copy", priority_queue_rbobject__initialize_copy, 1 );

  // PriorityQueue attributes
  rb_define_method( TspKit_PriorityQueue, "pq_size", priority_queue_rbobject__get_pq_size, 0 );
  rb_define_method( TspKit_PriorityQueue, "heap_root", priority_queue_rbobject__get_heap_root, 0 );
}
