// ext/tsp_kit/struct_weight_matrix.h

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definition for WeightMatrix and declarations for its memory management
//

#ifndef STRUCT_WEIGHT_MATRIX_H
#define STRUCT_WEIGHT_MATRIX_H

#include <ruby.h>
#include "narray.h"

typedef struct _weight_matrix_raw {
  int num_nodes;
  int *weights_shape;
  volatile VALUE narr_weights;
  double *weights;
  } WeightMatrix;

WeightMatrix *weight_matrix__create();

void weight_matrix__init( WeightMatrix *weight_matrix, int num_nodes );

void weight_matrix__destroy( WeightMatrix *weight_matrix );

void weight_matrix__gc_mark( WeightMatrix *weight_matrix );

void weight_matrix__deep_copy( WeightMatrix *weight_matrix_copy, WeightMatrix *weight_matrix_orig );

WeightMatrix * weight_matrix__clone( WeightMatrix *weight_matrix_orig );

#endif
