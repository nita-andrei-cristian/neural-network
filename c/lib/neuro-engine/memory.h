#ifndef MEMORY_MANAGEMENT_NN
#define MEMORY_MANAGEMENT_NN

#include "../utils.h"
#include "nodes.h"
#include "connections.h"

// note : weird edge case when one of the nodes is actually called 'connections'
_Bool ADD_DATA_FROM_RESPONSE(nodes_container *nodes_data, connections_container *connections_data, char* response);

void EXPORT_GRAPH(nodes_container* nodes, connections_container *connections, const char* directory);

_Bool SET_INCEPTION_GRAPH(nodes_container *nodes_data, connections_container *connections_data, char* path);


#endif
