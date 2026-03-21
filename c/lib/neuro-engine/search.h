#ifndef NN_SEARCH_ENGINE
#define NN_SEARCH_ENGINE

#include "nodes.h"
#include "connections.h"
#include "../utils.h"

node **GET_IMPORTANT_NODES(nodes_container *nodes, int threshold, int *out_count);
node **GET_IMPORTANT_NEIGHBOURS(connections_container *connections, long node_id, int threshold, int *count);
char *COMPUTE_IMPORTANT_NEIGHBOURS_RECURSIVE(connections_container *connections, node* root, int percent, int recursive, int *count);

#endif
