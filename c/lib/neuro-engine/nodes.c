#include "nodes.h"
#include "../utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* NewNode(const char *label, size_t label_length){
	if(nodes == NULL){
		fprintf(stderr, "Error : Skipped adding node because node conontainer is null.\n");
		return NULL;
	}
	if(label == NULL){
		fprintf(stderr, "Error : Skipped adding node because label is null.\n");
		return NULL;
	}

	if (nodes->count >= nodes->capacity){
		
		if (nodes->capacity == 0) nodes->capacity = 256;
		else nodes->capacity *= 2;
		
		Node* tmp = nodes->items;
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
	nodes->items[i].intensity = 1;

	nodes->count++;

	return &nodes->items[i];
}

Node* ReadNode(size_t i){
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

void free_nodes(){
	if (!nodes) return;
	if (nodes->items)
		free(nodes->items);
	free(nodes);
	nodes = NULL;
}

void init_nodes(){
	free_nodes();
	nodes =  malloc(sizeof(NodesContainer));
	if(!nodes){
		fprintf(stderr, "Error : Malloc failed, couldn't create nodes \n");
		return;
	}
	memset(nodes, 0, sizeof(*nodes));
}


Node* SearchNodeID(long target_id){
	for (int i = 0; i < nodes->count; i++){
		if (nodes->items[i].id == target_id){
			return &nodes->items[i];
		}
	}
	return NULL;
}

Node* SearchNodeLabel(const char* label){
	for (int i = 0; i < nodes->count; i++){
		if (strcmp(nodes->items[i].label, label)== 0){
			return &nodes->items[i];
		}
	}
	return NULL;
}
