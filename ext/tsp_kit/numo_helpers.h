#ifndef TSP_KIT_NUMO_HELPERS_H
#define TSP_KIT_NUMO_HELPERS_H

#include <ruby.h>
#include <numo/narray.h>
#include <numo/intern.h>

static inline VALUE
tsp_numo_new(VALUE klass, int rank, const int *dimensions)
{
  size_t *shape = ALLOCA_N(size_t, rank);
  int i;

  for (i = 0; i < rank; i++) {
    shape[i] = (size_t)dimensions[i];
  }
  return nary_new(klass, rank, shape);
}

static inline VALUE
tsp_numo_cast(VALUE klass, VALUE object)
{
  return rb_funcall(klass, rb_intern("cast"), 1, object);
}

static inline VALUE
tsp_numo_clone(VALUE object)
{
  return rb_funcall(object, rb_intern("clone"), 0);
}

static inline narray_t *
tsp_numo_metadata(VALUE object)
{
  narray_t *narray;

  GetNArray(object, narray);
  return narray;
}

static inline void *
tsp_numo_read_pointer(VALUE object)
{
  return na_get_pointer_for_read(object);
}

static inline void *
tsp_numo_write_pointer(VALUE object)
{
  return na_get_pointer_for_write(object);
}

static inline void *
tsp_numo_read_write_pointer(VALUE object)
{
  return na_get_pointer_for_read_write(object);
}

#endif
