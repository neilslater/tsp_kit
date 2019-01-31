// ext/tsp_kit/combined_nodes.c

#include "combined_nodes.h"

NodeType get_node_type_from_rv( VALUE rv_nodes ) {
  if ( TYPE(rv_nodes) == T_DATA && RDATA(rv_nodes)->dfree == (RUBY_DATA_FUNC)euclidean_nodes__destroy) {
    return NODE_EUC;
  }

  if ( TYPE(rv_nodes) == T_DATA && RDATA(rv_nodes)->dfree == (RUBY_DATA_FUNC)weight_matrix__destroy) {
    return NODE_WM;
  }

  rb_raise(rb_eArgError, "Unrecognised node subtype");

  // We don't reach here, but returning a valid NodeType prevents compiler warnings
  return NODE_EUC;
}
