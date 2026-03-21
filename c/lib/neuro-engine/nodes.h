#ifndef NODE_LIB
#define NODE_LIB

#define NODE_SIZE 32

#include <stdlib.h>
#include <math.h>

typedef struct node {
	char label[NODE_SIZE];	 // dynamic sizes yet to be implemented
	_Bool dead;
	long id;
	size_t label_length;
	double intensity;
} node;

typedef struct node_container {
	size_t capacity;
	size_t count;
	node *items;
} nodes_container;

node* NODES_ADD(nodes_container *nodes, const char *label, size_t label_length);

node* NODES_READ(nodes_container *nodes, size_t i);

nodes_container* NODES_NEW();
void NODES_FREE(nodes_container* nodes);

node* SEARCH_NODE_BY_ID(nodes_container *nodes, long id);

node* SEARCH_NODE_BY_LABEL(nodes_container *nodes, const char* label);


#endif

