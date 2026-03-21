#ifndef NN_SEARCH_ENGINE
#define NN_SEARCH_ENGINE

#include "nodes.h"
#include "../utils.h"

Node **GET_IMPORTANT_NODES(int threshold, int *out_count);
Node **GET_IMPORTANT_NEIGHBOURS(long node_id, int threshold, int *count);
char *COMPUTE_IMPORTANT_NEIGHBOURS_RECURSIVE(Node* root, int percent, int recursive, int *count);

#endif
