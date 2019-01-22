// ext/tsp_kit/shared_vars.h

#ifndef SHARED_VARS_H
#define SHARED_VARS_H

#define NUM2FLT(x) ((float)NUM2DBL(x))
#define FLT2NUM(x) (rb_float_new((double)x))

// Force inclusion of hash declarations (only Ruby MRI includes by default)
#ifdef HAVE_RUBY_ST_H
#include "ruby/st.h"
#else
#include "st.h"
#endif

extern VALUE TspKit;
extern VALUE TspKit_Nodes;
extern VALUE TspKit_EuclideanNodes;

extern VALUE TspKit_WeightMatrix;
extern VALUE TspKit_DistanceRank;
extern VALUE TspKit_Solution;
extern VALUE TspKit_GreedySolver;
extern VALUE TspKit_OneTree;
extern VALUE TspKit_PriorityQueue;

#endif
