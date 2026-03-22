#ifndef CONNECTIONS_LIB
#define CONNECTIONS_LIB

#include <stddef.h>
#include "nodes.h"

typedef struct connection {
	_Bool dead;
	long node1;
	long node2;
	double intensity;
	Node* pnode1;
	Node* pnode2;
} Connection;

typedef struct connections_container {
	size_t capacity;
	size_t count;
	Connection *items;
} ConnectionsContainer;

ConnectionsContainer* connections;

Connection* AddConnectionFromIDs(long node1, long node2, _Bool shouldDecay);

void init_connections();
void free_connections();

void DecayConnections();

Connection* findConnection(long node1, long node2);


#endif 
