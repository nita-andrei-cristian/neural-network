#ifndef NODE_LIB
#define NODE_LIB

#define NODE_SIZE 16

typedef struct {
	size_t label_length;
	char label[NODE_SIZE];	 // dynamic sizes yet to be implemented
	long id;
} node;

typedef struct {
	size_t capacity;
	size_t count;
	node *items;
} nodes_container;

void NODES_ADD(nodes_container *nodes, const char *label, size_t label_length);

node* NODES_READ(nodes_container *nodes, size_t i);

nodes_container* NODES_NEW();
void NODES_FREE(nodes_container* nodes){};

#endif

