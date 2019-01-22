// ext/tsp_kit/utilities.c

#include "utilities.h"

void quicksort_ids_by_double( int * ids, double * sort_by, int lowest, int highest ) {
  int pivot, j, i;
  int temp_id;

  if ( lowest < highest ) {
    pivot = lowest;
    i = lowest;
    j = highest;

    // Calculate pivot
    while ( i < j ) {
      while ( sort_by[ids[i]] <= sort_by[ids[pivot]] && i < highest )
        i++;
      while ( sort_by[ids[j]] > sort_by[ids[pivot]] )
        j--;
      if( i < j ) {
        temp_id = ids[i];
        ids[i] = ids[j];
        ids[j] = temp_id;
      }
    }

    temp_id = ids[pivot];
    ids[pivot] = ids[j];
    ids[j] = temp_id;

    quicksort_ids_by_double( ids, sort_by, lowest, j-1 );
    quicksort_ids_by_double( ids, sort_by, j+1, highest );
  }
}

void quicksort_ints( int * ids, int lowest, int highest ) {
  int pivot, j, i;
  int temp_id;

  if ( lowest < highest ) {
    pivot = lowest;
    i = lowest;
    j = highest;

    while ( i < j ) {
      while ( ids[i] <= ids[pivot] && i < highest )
        i++;
      while ( ids[j] > ids[pivot] )
        j--;
      if( i < j ) {
        temp_id = ids[i];
        ids[i] = ids[j];
        ids[j] = temp_id;
      }
    }

    temp_id = ids[pivot];
    ids[pivot] = ids[j];
    ids[j] = temp_id;

    quicksort_ints( ids, lowest, j-1 );
    quicksort_ints( ids, j+1, highest );
  }
}

// A Fisher-Yates shuffle
void shuffle_ints( int n, int *array ) {
  int i, tmp, r;
  for ( i = n-1; i >= 0; i-- ) {
    // This will be slightly biased for large n, but it is not a
    // noticeable issue for sizes of arrays used in TSP
    r = genrand_int31() % ( i + 1 );
    tmp = array[r];
    array[r] = array[i];
    array[i] = tmp;
  }
  return;
}
