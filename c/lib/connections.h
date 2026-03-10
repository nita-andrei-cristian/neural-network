#ifndef CONNECTIONS_LIB
#define CONNECTIONS_LIB

#include <stddef.h>
#include <nodes.h>

typedef struct connection {
	long node1;
	long node2;
} connection;

typedef struct connections_container {
	size_t capacity;
	size_t count;
	connection *items;
} connections_container;

void CONNECTIONS_ADD_FROM_IDS(connections_container *connections, long node1, long node2);
void CONNECTIONS_ADD_FROM_MEMORY(connections_container *connections, node* node1, node* node2);

void CONNECTIONS_FREE(connections_container* connections);

connections_container* CONNECTIONS_NEW();

#endif 
