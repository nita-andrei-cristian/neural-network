#ifndef CONNECTIONS_LIB
#define CONNECTIONS_LIB

#include <stddef.h>

typedef struct connection {
	long node1;
	long node2;
} connection;

typedef struct connections_container {
	size_t capacity;
	size_t count;
	connection *items;
} connections_container;

void CONNECTIONS_ADD(connections_container *connections, long node1, long node2);

#endif 
