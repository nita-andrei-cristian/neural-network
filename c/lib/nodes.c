#include <nodes.h>
#include <utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node* NODES_ADD(nodes_container *nodes, const char *label, size_t label_length){
	if(nodes == NULL){
		fprintf(stderr, "Error : Skipped adding node because nodes_container is null.\n");
		return NULL;
	}
	if(label == NULL){
		fprintf(stderr, "Error : Skipped adding node because label is null.\n");
		return NULL;
	}

	if (nodes->count >= nodes->capacity){
		
		if (nodes->capacity == 0) nodes->capacity = 256;
		else nodes->capacity *= 2;
		
		node* tmp = nodes->items;
		nodes->items = realloc(nodes->items, nodes->capacity * sizeof(*nodes->items));
		if (!nodes->items){
			printf("Error : Memory re-allocation failed. Will not store item %s. \n", label);
			nodes->items = tmp;
			return NULL;
		}
	}

	int i = nodes->count;
	label_length = min(label_length,15);
	memcpy(nodes->items[i].label, label, label_length);
	nodes->items[i].label_length = label_length;
	nodes->items[i].label[label_length] = '\0'; 
	nodes->items[i].id = rand();

	nodes->count++;

	return &nodes->items[i];
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


node* SEARCH_NODE_BY_ID(nodes_container *nodes, long target_id){
	for (int i = 0; i < nodes->count; i++){
		if (nodes->items[i].id == target_id){
			return &nodes->items[i];
		}
	}
	return NULL;
}

node* SEARCH_NODE_BY_LABEL(nodes_container *nodes, const char* label){
	for (int i = 0; i < nodes->count; i++){
		if (strcmp(nodes->items[i].label, label)== 0){
			return &nodes->items[i];
		}
	}
	return NULL;
}
