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

Node* NewNode(const char *label, size_t label_length);

Node* ReadNode(size_t i);

void init_nodes();
void free_nodes();

Node* SearchNodeID(long id);

Node* SearchNodeLabel(const char* label);


#endif

