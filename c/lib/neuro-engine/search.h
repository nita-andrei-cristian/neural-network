#ifndef NN_SEARCH_ENGINE
#define NN_SEARCH_ENGINE

#include "nodes.h"
#include "../utils.h"

/* Filters globally nodes based on a threshold 1-100  */
Node **GetNodes(int threshold, int *out_count);

/* Filters neighbour nodes based on a threshold 1-100  */
Node **GetNodeNeighbours(long node_id, int threshold, int *count);

/* Filters neighbour nodes based on a threshold 1-100 but recursively  */
char *ComputeNodeFamily(Node* root, int threshold, int depth, int *count);

#endif
