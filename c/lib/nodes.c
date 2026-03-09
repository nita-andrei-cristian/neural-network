#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nodes.h>

void NODES_ADD(nodes_container *nodes, const char *label, size_t label_length){
	if(nodes == NULL){
		fprintf(stderr, "Error : Skipped adding node because nodes_container is null.\n");
		return;
	}
	if(label == NULL){
		fprintf(stderr, "Error : Skipped adding node because label is null.\n");
		return;
	}

	if (nodes->count >= nodes->capacity){
		
		if (nodes->capacity == 0) nodes->capacity = 256;
		else nodes->capacity *= 2;
		
		node* tmp = nodes->items;
		nodes->items = realloc(nodes->items, nodes->capacity * sizeof(*nodes->items));
		if (!nodes->items){
			printf("Error : Memory re-allocation failed. Will not store item %s. \n", label);
			nodes->items = tmp;
			return;
		}
	}

	int i = nodes->count;
	label_length = min(label_length,15);
	memcpy(nodes->items[i].label, label, label_length);
	nodes->items[i].label_length = label_length;
	nodes->items[i].label[label_length] = '\0'; 

	nodes->count++;
}

node* NODES_READ(nodes_container *nodes, size_t i){
	if (!nodes){
		fprintf(stderr, "Error : nodes is NULL.\n");
		return NULL;
	}
	if (i >= nodes->count){
		fprintf(stderr, "Error : Index out of range.\n");
		return NULL;
	};
	return &nodes->items[i];
}

void NODES_FREE(nodes_container* nodes){
	free(nodes->items);
	free(nodes);
}

nodes_container* NODES_NEW(){
	nodes_container* n =  malloc(sizeof(nodes_container));
	if(!n){
		fprintf(stderr, "Error : Malloc failed, couldn't create nodes \n");
		return NULL;
	}
	memset(n,0, sizeof(*n));
	return n;
}
