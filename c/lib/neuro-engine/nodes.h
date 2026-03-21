#ifndef NODE_LIB
#define NODE_LIB

#define NODE_SIZE 32

#include <stdlib.h>

typedef struct node {
	char label[NODE_SIZE];	 // dynamic sizes yet to be implemented
	_Bool dead;
	long id;
	size_t label_length;
	double intensity;
} Node;

typedef struct NodesContainer {
	size_t capacity;
	size_t count;
	Node *items;
} NodesContainer;

struct NodesContainer* nodes; 

Node* NODES_ADD(const char *label, size_t label_length);

Node* NODES_READ(size_t i);

void NODES_NEW();
void NODES_FREE();

Node* SEARCH_NODE_BY_ID(long id);

Node* SEARCH_NODE_BY_LABEL(const char* label);


#endif

