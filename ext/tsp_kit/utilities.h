// ext/tsp_kit/utilities.h

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Declarations of narray helper functions
//

#ifndef CORE_UTILITIES_H
#define CORE_UTILITIES_H

#include <math.h>
#include <stdbool.h>
#include "mt.h"

void quicksort_ids_by_double( int * ids, double * sort_by, int lowest, int highest );
void quicksort_ints( int * ids, int lowest, int highest );
void shuffle_ints( int n, int *array );

#endif
