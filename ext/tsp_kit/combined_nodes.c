// ext/tsp_kit/combined_nodes.c

#include "combined_nodes.h"

double null_distance_fn( void *nodes, int node_a_id, int node_b_id ) {
  return 0.0;
}

void null_distance_array_fn( void *nodes, int node_id, double * distances ) {
  distances[node_id] = 0.0;
  return;
}

NodeType get_node_type_from_rv( VALUE rv_nodes ) {
  if ( TYPE(rv_nodes) == T_DATA && RDATA(rv_nodes)->dfree == (RUBY_DATA_FUNC)euclidean_nodes__destroy) {
    return NODE_EUC;
  }

  if ( TYPE(rv_nodes) == T_DATA && RDATA(rv_nodes)->dfree == (RUBY_DATA_FUNC)cost_matrix__destroy) {
    return NODE_CM;
  }

  rb_raise(rb_eArgError, "Unrecognised node subtype");

  // We don't reach here, but returning a valid NodeType prevents compiler warnings
  return NODE_EUC;
}

DISTANCEFN distance_fn_of( NodeType node_type ) {
  switch(node_type) {
    case NODE_EUC:
      return euclidean_nodes__distance_between;
    case NODE_CM:
      return cost_matrix__distance_between;
  }
  return null_distance_fn;
}

DISTANCEARRAYFN distance_array_fn_of( NodeType node_type ) {
  switch(node_type) {
    case NODE_EUC:
      return euclidean_nodes__all_distances_from;
    case NODE_CM:
      return cost_matrix__all_distances_from;
  }
  return null_distance_array_fn;
}