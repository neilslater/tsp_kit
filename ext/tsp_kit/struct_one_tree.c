// ext/tsp_kit/struct_one_tree.c

#include "struct_one_tree.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Definitions for OneTree memory management
//

OneTree *one_tree__create() {
  OneTree *tree;
  tree = xmalloc( sizeof(OneTree) );
  tree->num_nodes = 0;
  tree->current_tree_idx = 0;
  tree->node_penalties_shape = NULL;
  tree->narr_node_penalties = Qnil;
  tree->node_penalties = NULL;
  tree->node_ids_shape = NULL;
  tree->narr_node_ids = Qnil;
  tree->node_ids = NULL;
  tree->parents_shape = NULL;
  tree->narr_parents = Qnil;
  tree->parents = NULL;
  tree->tree_idx_of_node = NULL;
  tree->second_best_penalty_distance = NULL;
  tree->best_penalty_node_id = NULL;
  tree->second_best_penalty_node_id = NULL;
  tree->second_best_penalty_distance = NULL;
  tree->d = NULL;

  tree->nodes = NULL;
  tree->distance_fn = null_distance_fn;
  tree->dr_max_rank = 0;
  tree->dr_closest_nodes = NULL;
  return tree;
}

void one_tree__init( OneTree *tree, int num_nodes ) {
  int i;
  struct NARRAY *narr;
  double *narr_node_penalties_ptr;
  int32_t *narr_node_ids_ptr;
  int32_t *narr_parents_ptr;

  tree->num_nodes = num_nodes;

  tree->dr_max_rank = -1;

  tree->tree_idx_of_node = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    tree->tree_idx_of_node[i] = 0;
  }

  tree->d = ALLOC_N( int, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    tree->d[i] = 0;
  }

  tree->best_penalty_node_id = ALLOC_N( int, num_nodes );
  tree->second_best_penalty_node_id = ALLOC_N( int, num_nodes );
  tree->second_best_penalty_distance = ALLOC_N( double, num_nodes );
  for( i = 0; i < num_nodes; i++ ) {
    tree->best_penalty_node_id[i] = -1;
    tree->second_best_penalty_node_id[i] = -1;
    tree->second_best_penalty_distance[i] = -DBL_MAX;
  }

  tree->node_penalties_shape = ALLOC_N( int, 1 );
  tree->node_penalties_shape[0] = num_nodes;
  tree->narr_node_penalties = na_make_object( NA_DFLOAT, 1, tree->node_penalties_shape, cNArray );
  GetNArray( tree->narr_node_penalties, narr );
  narr_node_penalties_ptr = (double*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_penalties_ptr[i] = 0.0;
  }
  tree->node_penalties = (double *) narr->ptr;

  tree->node_ids_shape = ALLOC_N( int, 1 );
  tree->node_ids_shape[0] = num_nodes + 2;
  tree->narr_node_ids = na_make_object( NA_LINT, 1, tree->node_ids_shape, cNArray );
  GetNArray( tree->narr_node_ids, narr );
  narr_node_ids_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_node_ids_ptr[i] = -1;
  }
  tree->node_ids = (int32_t *) narr->ptr;

  tree->parents_shape = ALLOC_N( int, 1 );
  tree->parents_shape[0] = num_nodes + 2;
  tree->narr_parents = na_make_object( NA_LINT, 1, tree->parents_shape, cNArray );
  GetNArray( tree->narr_parents, narr );
  narr_parents_ptr = (int32_t*) narr->ptr;
  for( i = 0; i < narr->total; i++ ) {
    narr_parents_ptr[i] = -1;
  }
  tree->parents = (int32_t *) narr->ptr;

  return;
}

void one_tree__destroy( OneTree *tree ) {
  xfree( tree->tree_idx_of_node );
  xfree( tree->best_penalty_node_id );
  xfree( tree->second_best_penalty_node_id );
  xfree( tree->second_best_penalty_distance );
  xfree( tree->d );
  xfree( tree->dr_closest_nodes );
  xfree( tree->node_penalties_shape );
  xfree( tree->node_ids_shape );
  xfree( tree->parents_shape );
  xfree( tree );
  return;
}

void one_tree__gc_mark( OneTree *tree ) {
  rb_gc_mark( tree->narr_node_penalties );
  rb_gc_mark( tree->narr_node_ids );
  rb_gc_mark( tree->narr_parents );
  return;
}

void one_tree__deep_copy( OneTree *tree_copy, OneTree *tree_orig ) {
  struct NARRAY *narr;
  int num_nodes = tree_orig->num_nodes;

  tree_copy->num_nodes = num_nodes;
  tree_copy->dr_max_rank = tree_orig->dr_max_rank;

  tree_copy->narr_node_penalties = na_clone( tree_orig->narr_node_penalties );
  GetNArray( tree_copy->narr_node_penalties, narr );
  tree_copy->node_penalties = (double *) narr->ptr;
  tree_copy->node_penalties_shape = ALLOC_N( int, 1 );
  memcpy( tree_copy->node_penalties_shape, tree_orig->node_penalties_shape, 1 * sizeof(int) );

  tree_copy->narr_node_ids = na_clone( tree_orig->narr_node_ids );
  GetNArray( tree_copy->narr_node_ids, narr );
  tree_copy->node_ids = (int32_t *) narr->ptr;
  tree_copy->node_ids_shape = ALLOC_N( int, 1 );
  memcpy( tree_copy->node_ids_shape, tree_orig->node_ids_shape, 1 * sizeof(int) );

  tree_copy->narr_parents = na_clone( tree_orig->narr_parents );
  GetNArray( tree_copy->narr_parents, narr );
  tree_copy->parents = (int32_t *) narr->ptr;
  tree_copy->parents_shape = ALLOC_N( int, 1 );
  memcpy( tree_copy->parents_shape, tree_orig->parents_shape, 1 * sizeof(int) );

  tree_copy->tree_idx_of_node = ALLOC_N( int, num_nodes );
  memcpy( tree_copy->tree_idx_of_node, tree_orig->tree_idx_of_node, ( num_nodes ) * sizeof(int) );

  tree_copy->d = ALLOC_N( int, num_nodes );
  memcpy( tree_copy->d, tree_orig->d, ( num_nodes ) * sizeof(int) );

  tree_copy->dr_closest_nodes = ALLOC_N( int, 20 );
  memcpy( tree_copy->dr_closest_nodes, tree_orig->dr_closest_nodes, ( 20 ) * sizeof(int) );

  return;
}

OneTree *one_tree__clone( OneTree *tree_orig ) {
  OneTree *tree_copy = one_tree__create();
  one_tree__deep_copy( tree_copy, tree_orig );
  return tree_copy;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//   nodes/node_id refers to source graph, distance ranks
//   To differentiate, the index into node_ids and parents is "tree_idx"

void one_tree__connect_data_sources( OneTree *tree, NodeType ntype, void *nodes, DistanceRank *dr ) {
  tree->nodes = nodes;
  tree->distance_fn = distance_fn_of( ntype );
  tree->dr_max_rank = dr->max_rank;
  tree->dr_closest_nodes = dr->closest_nodes;
  return;
}

PriorityQueue * one_tree__fresh_priority_queue( OneTree *tree ) {
  PriorityQueue *pq = priority_queue__create();
  priority_queue__init( pq, tree->num_nodes );
  return pq;
}

void one_tree__set_tree_idx_one( OneTree *tree ) {
  int tree_idx_one_node_id, before_node_id, max_from_node_id;
  double distance;
  double max_distance = 0.0;
  max_from_node_id = 0;
  tree_idx_one_node_id = 1;

  one_tree__calc_degrees( tree );

  // Find a single-degree node with maximum second-distance length
  // We use regular cost here, but should probably be penalised cost (which we should probably stash from earlier calculations)
  for (int i = 0; i < tree->num_nodes; i++) {
    // i -> to_node_id
    if (tree->d[i] != 1) continue;

    distance = tree->second_best_penalty_distance[i];

    if (distance > max_distance) {
      max_distance = distance;
      max_from_node_id = tree->second_best_penalty_node_id[i];
      tree_idx_one_node_id = i;
    }
  }

  before_node_id = tree->tree_idx_of_node[tree_idx_one_node_id];

  // Add to node 1 position (in two places so both links are stored)
  tree->tree_idx_of_node[tree_idx_one_node_id] = 1;
  tree->node_ids[0] = tree_idx_one_node_id;
  tree->node_ids[1] = tree_idx_one_node_id;
  if (before_node_id == 2) {
    // This means node 3 is only connection to node 2, and we should start on node 3
    tree->parents[0] = 3;
    tree->parents[3] = -1;
  } else {
    tree->parents[0] = tree->parents[before_node_id];
  }

  tree->parents[1] = tree->tree_idx_of_node[max_from_node_id];

  // For some reason the node might already by linked by its second-best link . . .
  if ( tree->parents[0] == tree->parents[1] ) {
    tree->parents[1] = tree->tree_idx_of_node[ tree->best_penalty_node_id[tree_idx_one_node_id] ];
  }

  // Remove from old position
  tree->node_ids[before_node_id] = -1;
  tree->parents[before_node_id] = -1;

  // Update degrees
  tree->d[tree_idx_one_node_id] = 2;

  return;
}

void one_tree__enqueue_priority_links_to( OneTree *tree, PriorityQueue *pq, int to_node_id, int to_tree_idx ) {
  // Work through nearest cities list and enqueue links to this city based on penalised distance
  double distance; // Not to be confused with non-prime penalty elsewhere, this is the adjusted length for finding better spanning tree
  int from_node_id;
  int dr_offset = to_node_id * tree->dr_max_rank;

  // These need to be tracked in order to connect Node 1 at the end.
  double best_distance = DBL_MAX * 0.99;
  double second_best_distance = DBL_MAX;
  int best_from_node_id = -1;
  int second_best_from_node_id = -1;

  for (int i = 0; i < tree->dr_max_rank; i++) {
    from_node_id = tree->dr_closest_nodes[dr_offset+i];
    // We might be using a reciprocated list where entries can be -1 signifying list ends early
    if (from_node_id < 0) break;

    distance = tree->node_penalties[to_node_id] + tree->node_penalties[from_node_id]
        + tree->distance_fn( tree->nodes, from_node_id, to_node_id );

    if (distance < second_best_distance) {
      if (distance < best_distance) {
        second_best_distance = best_distance;
        second_best_from_node_id = best_from_node_id;
        best_distance = distance;
        best_from_node_id = from_node_id;
      } else {
        second_best_distance = distance;
        second_best_from_node_id = from_node_id;
      }
    }

    // TODO: Should to_node_id be to_tree_idx here? Should this be inside conditional?
    priority_queue__push( pq, from_node_id, distance, to_tree_idx );
  }

  tree->best_penalty_node_id[to_node_id] = best_from_node_id;
  tree->second_best_penalty_node_id[to_node_id] = second_best_from_node_id;
  tree->second_best_penalty_distance[to_node_id] = second_best_distance;

  return;
}

int one_tree__add_next_priority_link_to_tree( OneTree *tree, PriorityQueue *pq ) {
  int node_id, parent_tree_idx, tree_idx;

  tree->current_tree_idx++;

  parent_tree_idx = priority_queue__peek_payload( pq );
  node_id = priority_queue__pop( pq );
  if ( node_id < 0 ) return node_id;

  tree_idx = tree->current_tree_idx;

  tree->node_ids[tree_idx] = node_id;
  tree->parents[tree_idx] = parent_tree_idx;
  tree->tree_idx_of_node[node_id] = tree_idx;

  one_tree__enqueue_priority_links_to( tree, pq, node_id, tree_idx );

  return node_id;
}

void one_tree__reset( OneTree *tree ) {
  int i;
  int num_nodes = tree->num_nodes;

  // Tree index arrays
  for( i = 0; i < num_nodes+2; i++ ) {
    tree->node_ids[i] = -1;
    tree->parents[i] = -1;
  }

  // Node arrays
  for( i = 0; i < num_nodes; i++ ) {
    tree->tree_idx_of_node[i] = 0;
    tree->d[i] = 0;
    tree->best_penalty_node_id[i] = -1;
    tree->second_best_penalty_node_id[i] = -1;
    tree->second_best_penalty_distance[i] = DBL_MAX;
  }

  tree->current_tree_idx = 0;

  return;
}

void one_tree__build( OneTree *tree, NodeType ntype, void *nodes, DistanceRank *dr, int start_node_id ) {
  int node_id;
  PriorityQueue *pq;

  one_tree__connect_data_sources( tree, ntype, nodes, dr );
  one_tree__reset( tree );

  pq = one_tree__fresh_priority_queue( tree );

  tree->current_tree_idx = 1; // Although node 1 left empty until the end, it takes slots 0 and 1

  node_id = start_node_id;
  priority_queue__push( pq, node_id, 0.0, -1 );

  do {
    node_id = one_tree__add_next_priority_link_to_tree( tree, pq );
  } while ( node_id > -1 );

  priority_queue__destroy( pq );

  one_tree__set_tree_idx_one( tree );

  return;
}

double one_tree__length( OneTree *tree, NodeType ntype, void *nodes ) {
  int from_node_id, to_node_id, parent_node_id, skip_count;
  double l = 0.0;

  skip_count = 0;
  tree->nodes = nodes;
  tree->distance_fn = distance_fn_of( ntype );

  for(int i = 0; i < (tree->num_nodes + 2); i++) {
    from_node_id = tree->node_ids[i];
    parent_node_id = tree->parents[i];
    if (from_node_id < 0 || parent_node_id < 0) {
      skip_count++;
      continue;
    }
    to_node_id = tree->node_ids[parent_node_id];
    l += tree->distance_fn( tree->nodes, from_node_id, to_node_id );
  }

  // We are allowed one for start node, and one for node 1 being moved.
  if (skip_count > 2) {
    l = 0.0;
  }

  return l;
}

double one_tree__w_length( OneTree *tree, NodeType ntype, void *nodes ) {
  int from_node_id, to_node_id, parent_node_id, skip_count;
  double l = 0.0;

  skip_count = 0;
  tree->nodes = nodes;
  tree->distance_fn = distance_fn_of( ntype );

  for(int i = 0; i < (tree->num_nodes + 2); i++) {
    from_node_id = tree->node_ids[i];
    parent_node_id = tree->parents[i];
    if (from_node_id < 0 || parent_node_id < 0) {
      skip_count++;
      continue;
    }
    to_node_id = tree->node_ids[parent_node_id];
    l += tree->node_penalties[to_node_id] + tree->node_penalties[from_node_id]
        + tree->distance_fn( tree->nodes, from_node_id, to_node_id );
  }

  // We are allowed one for start node, and one for node 1 being moved.
  if (skip_count > 2) {
    return 0.0;
  }

  for(int i = 0; i < (tree->num_nodes); i++) {
    l -= 2* tree->node_penalties[i];
  }

  return l;
}

void one_tree__calc_degrees( OneTree *tree ) {
  int from_node_id, to_node_id, parent_node_id;

  for(int i = 0; i < tree->num_nodes; i++) {
    tree->d[i] = 0;
  }

  for(int i = 0; i < (tree->num_nodes + 2); i++) {
    from_node_id = tree->node_ids[i];
    parent_node_id = tree->parents[i];
    if (from_node_id < 0 || parent_node_id < 0) continue;
    to_node_id = tree->node_ids[parent_node_id];
    tree->d[from_node_id]++;
    tree->d[to_node_id]++;
  }

  return;
}

void one_tree__reindex( OneTree *tree ) {
  for(int i = 1; i < (tree->num_nodes + 2); i++) {
    if (tree->node_ids[i] < 0) continue;
    tree->tree_idx_of_node[ tree->node_ids[i] ] = i;
  }
  return;
}


inline double one_tree__alpha_distance( OneTree *tree, int node_a_id, int node_b_id, int use_penalties ) {
  double d;

  d = tree->distance_fn( tree->nodes, node_a_id, node_b_id );

  if (use_penalties) {
    d += tree->node_penalties[node_a_id] + tree->node_penalties[node_b_id];
  }

  return d;
}

// From http://akira.ruc.dk/~keld/research/LKH/LKH-1.3/DOC/LKH_REPORT.pdf page 23.
void one_tree__generate_alpha_ranking( OneTree *tree, NodeType ntype, void *nodes, DistanceRank *target_dr, int use_penalties ) {
  int i, j, k, n, root_tree_idx; // Refer to node_ids
  int *mark;
  double *b;
  int *node_ids_buffer; // For sorting
  double *alpha_buffer; // For sorting by city_id
  int tree_idx_one_node_id, from_node_id, to_node_id, p, dr_offset;
  double c, tree_idx_one_longest_c;

  // NB: This links us to a *writeable* dr instance, do not call any of: build, set_tree_idx_one, enqueue_priority_links_to
  one_tree__connect_data_sources( tree, ntype, nodes, target_dr );

  tree_idx_one_node_id = tree->node_ids[1];
  tree_idx_one_longest_c = one_tree__alpha_distance( tree, tree_idx_one_node_id, tree->node_ids[tree->parents[1]], use_penalties );
  c = one_tree__alpha_distance( tree, tree_idx_one_node_id, tree->node_ids[tree->parents[0]], use_penalties );
  tree_idx_one_longest_c = ( (tree_idx_one_longest_c  > c) ? tree_idx_one_longest_c : c );

  // This algorithm from http://akira.ruc.dk/~keld/research/LKH/LKH-1.3/DOC/LKH_REPORT.pdf, page 23
  alpha_buffer = ALLOC_N( double, tree->num_nodes );
  node_ids_buffer = ALLOC_N( int, tree->num_nodes );
  for( i = 0; i < tree->num_nodes; i++ ) {
    alpha_buffer[i] = 0.0;
    node_ids_buffer[i] = i;
  }

  root_tree_idx = 2;
  if (tree->node_ids[2] == -1 ) {
    root_tree_idx = 3;
  }
  n = tree->num_nodes + 2; // Uses tree_idx 1-indexed. Also expect one node_id to be -1 representing node that was moved.
  b = ALLOC_N( double, n );
  mark = ALLOC_N( int, n );

  for( i = root_tree_idx; i < n; i++ ) {
    mark[i] = 0;
  }

  for( i = root_tree_idx; i < n; i++ ) {
    if (tree->node_ids[i] < 0) continue;
    b[i] = -DBL_MAX;

    for ( k = i; k != root_tree_idx; k = j ) {
      j = tree->parents[k];
      from_node_id = tree->node_ids[k];
      to_node_id = tree->node_ids[j];
      c = one_tree__alpha_distance( tree, from_node_id, to_node_id, use_penalties );
      b[j] = (b[k] > c ? b[k] : c);
      mark[j] = i;
    }

    for( j = root_tree_idx; j < n; j++ ) {
      if (tree->node_ids[j] < 0) continue;
      if ( j != i ) {
        if ( mark[j] != i ) {
          from_node_id = tree->node_ids[j];
          to_node_id = tree->node_ids[tree->parents[j]];
          c = one_tree__alpha_distance( tree, from_node_id, to_node_id, use_penalties );
          b[j] = (b[tree->parents[j]] > c ? b[tree->parents[j]] : c);
        }

        from_node_id = tree->node_ids[i];
        to_node_id = tree->node_ids[j];
        c = one_tree__alpha_distance( tree, from_node_id, to_node_id, use_penalties );
        alpha_buffer[to_node_id] = 1.00001 * c - b[j];  // Extra 0.00001 is to allow secondary sorting by distance
      }
    }

    // Figure out alpha distance from node i to node 1 . . .
    from_node_id = tree->node_ids[i];
    alpha_buffer[from_node_id] = -DBL_MAX;

    if ( tree->parents[0] == i || tree->parents[1] == i ) {
      alpha_buffer[tree_idx_one_node_id] = 0.00001 * one_tree__alpha_distance( tree, from_node_id, to_node_id, use_penalties );
    } else {
      c = one_tree__alpha_distance( tree, from_node_id, to_node_id, use_penalties );
      alpha_buffer[tree_idx_one_node_id] = 1.00001 * c - tree_idx_one_longest_c;
    }

    // Sort and transfer to distance ranks . . .
    quicksort_ids_by_double( node_ids_buffer, alpha_buffer, 0, tree->num_nodes - 1 );
    dr_offset = tree->dr_max_rank * from_node_id - 1;
    for ( p = 1; p <= tree->dr_max_rank; p++ ) {
      tree->dr_closest_nodes[ dr_offset + p ] = node_ids_buffer[p];
    }

    printf("Alpha ranking for tree_idx %d, node %d. Nodes [%d, %d, %d, %d, %d ...]\n",
            i, from_node_id, node_ids_buffer[1], node_ids_buffer[2],
            node_ids_buffer[3], node_ids_buffer[4], node_ids_buffer[5]
            );
  }

  // A simpler calculation for Node 1 at the end
  alpha_buffer[tree_idx_one_node_id] = -DBL_MAX;
  for( i = 0; i < tree->num_nodes; i++) {
    if (i==tree_idx_one_node_id) continue;

    if ( tree->parents[0] == tree->tree_idx_of_node[i] || tree->parents[1] == tree->tree_idx_of_node[i] ) {
      alpha_buffer[i] = 0.00001 * one_tree__alpha_distance( tree, i, tree_idx_one_node_id, use_penalties );
    } else {
      c = one_tree__alpha_distance( tree, i, tree_idx_one_node_id, use_penalties );
      alpha_buffer[i] = 1.00001 * c - tree_idx_one_longest_c;
    }
  }

  quicksort_ids_by_double( node_ids_buffer, alpha_buffer, 0, tree->num_nodes - 1 );
  dr_offset = tree->dr_max_rank * tree_idx_one_node_id - 1;
  for ( p = 1; p <= tree->dr_max_rank; p++ ) {
    tree->dr_closest_nodes[ dr_offset + p ] = node_ids_buffer[p];
  }

  printf("Alpha ranking for tree_idx %d, node %d. Nodes [%d, %d, %d, %d, %d ...]\n",
          1, tree_idx_one_node_id, node_ids_buffer[1], node_ids_buffer[2],
          node_ids_buffer[3], node_ids_buffer[4], node_ids_buffer[5]
          );

  xfree(mark);
  xfree(b);
  xfree(alpha_buffer);
  xfree(node_ids_buffer);

  return;
}
