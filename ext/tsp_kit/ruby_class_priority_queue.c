// ext/tsp_kit/ruby_class_priority_queue.c

#include "ruby_class_priority_queue.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ruby bindings for the single-pass Prim queue - the deeper implementation is in
//  struct_priority_queue.c
//

static const rb_data_type_t priority_queue_data_type = {
  "TspKit::PriorityQueue",
  { (RUBY_DATA_FUNC)priority_queue__gc_mark, (RUBY_DATA_FUNC)priority_queue__destroy, NULL, NULL,
    { NULL } },
  NULL, NULL, RUBY_TYPED_FREE_IMMEDIATELY
};

VALUE priority_queue_as_ruby_class( PriorityQueue *priority_queue , VALUE klass ) {
  return TypedData_Wrap_Struct( klass, &priority_queue_data_type, priority_queue );
}

VALUE priority_queue_alloc(VALUE klass) {
  return priority_queue_as_ruby_class( priority_queue__create(), klass );
}

PriorityQueue *get_priority_queue_struct( VALUE obj ) {
  PriorityQueue *priority_queue;
  TypedData_Get_Struct( obj, PriorityQueue, &priority_queue_data_type, priority_queue );
  return priority_queue;
}

void assert_value_wraps_priority_queue( VALUE obj ) {
  if (!rb_typeddata_is_kind_of(obj, &priority_queue_data_type)) {
    rb_raise( rb_eTypeError, "Expected a PriorityQueue object, but got something else" );
  }
}

/* Document-class: TspKit::PriorityQueue
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Queue method definitions
//

/* @overload initialize( pq_size )
 * Creates an empty single-pass queue with positive capacity.
 * @param [Integer] pq_size positive capacity representable as a C int
 * @return [TspKit::Utils::PriorityQueue] new queue
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
 * Fixed queue capacity
 * @return [Integer]
 */
VALUE priority_queue_rbobject__get_pq_size( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue->pq_size );
}

/* @!attribute [r] heap_root
 * Minimum-priority ID, or -1 when empty
 * @return [Integer]
 */
VALUE priority_queue_rbobject__get_heap_root( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue->heap_root );
}

////////////////////////////////////////////////////////////////////////////////////////////////////


/* @overload push( id, priority, payload )
 * Inserts an unseen ID or decreases its key; equal keys replace payloads.
 * Larger keys and settled IDs are ignored. Equal priorities use smaller IDs first.
 * @param [Integer] id identifier in 0...pq_size
 * @param [Float] priority finite key
 * @param [Integer] payload associated C int value
 * @return [TspKit::Utils::PriorityQueue] self
 */
VALUE priority_queue_rbobject__push( VALUE self, VALUE rv_id, VALUE rv_priority, VALUE rv_payload  ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  priority_queue__push( priority_queue, NUM2INT( rv_id ), NUM2DBL( rv_priority ), NUM2INT( rv_payload ) );
  return self;
}

/* @overload pop( )
 * Returns highest-priority element, removing it from the queue
 * @return [Integer]
 */
VALUE priority_queue_rbobject__pop( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue__pop( priority_queue ) );
}

/* @overload peek( )
 * Returns highest-priority element, but does not remove it from the queue
 * @return [Integer]
 */
VALUE priority_queue_rbobject__peek( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue__peek( priority_queue ) );
}

/* @overload peek_priority( )
 * Returns value of highest-priority element, but does not remove it from the queue
 * @return [Float]
 */
VALUE priority_queue_rbobject__peek_priority( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return DBL2NUM( priority_queue__peek_priority( priority_queue ) );
}

/* @overload peek_payload( )
 * Returns payload of highest-priority element, but does not remove it from the queue
 * @return [Float]
 */
VALUE priority_queue_rbobject__peek_payload( VALUE self ) {
  PriorityQueue *priority_queue = get_priority_queue_struct( self );
  return INT2NUM( priority_queue__peek_payload( priority_queue ) );
}

void init_priority_queue_class( ) {
  // PriorityQueue instantiation and class methods
  rb_define_alloc_func( TspKit_PriorityQueue, priority_queue_alloc );
  rb_define_method( TspKit_PriorityQueue, "initialize", priority_queue_rbobject__initialize, 1 );
  rb_define_method( TspKit_PriorityQueue, "initialize_copy", priority_queue_rbobject__initialize_copy, 1 );

  // PriorityQueue attributes
  rb_define_method( TspKit_PriorityQueue, "pq_size", priority_queue_rbobject__get_pq_size, 0 );
  rb_define_method( TspKit_PriorityQueue, "heap_root", priority_queue_rbobject__get_heap_root, 0 );

  // PriorityQueue methods (really these are just for testing, PriorityQueue is used internally only)
  rb_define_method( TspKit_PriorityQueue, "push", priority_queue_rbobject__push, 3 );
  rb_define_method( TspKit_PriorityQueue, "pop", priority_queue_rbobject__pop, 0 );
  rb_define_method( TspKit_PriorityQueue, "peek", priority_queue_rbobject__peek, 0 );
  rb_define_method( TspKit_PriorityQueue, "peek_priority", priority_queue_rbobject__peek_priority, 0 );
  rb_define_method( TspKit_PriorityQueue, "peek_payload", priority_queue_rbobject__peek_payload, 0 );
}
