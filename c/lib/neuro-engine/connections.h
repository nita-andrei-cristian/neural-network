#ifndef CONNECTIONS_LIB
#define CONNECTIONS_LIB

#include <stddef.h>
#include "nodes.h"

typedef struct connection {
	bool dead;
	long node1;
	long node2;
	double intensity;
	node* pnode1;
	node* pnode2;
} connection;

typedef struct connections_container {
	size_t capacity;
	size_t count;
	connection *items;
} connections_container;

connection* CONNECTIONS_ADD_FROM_IDS(connections_container *connections, nodes_container *nodes, long node1, long node2, bool shouldDecay);

void CONNECTIONS_FREE(connections_container* connections);

void CONNECTIONS_DECAY(connections_container* connections);

connection* CONNECTIONS_SEARCH_BY_NODES(connections_container* connections, long node1, long node2);

connections_container* CONNECTIONS_NEW();

#endif 
