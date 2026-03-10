#include <connections.h>
#include <nodes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CONNECTIONS_ADD_FROM_IDS(connections_container *connections, long node1, long node2){
	if(connections == NULL){
		fprintf(stderr, "Error : Skipped adding connection because nodes_container is null.\n");
		return;
	}
	
	if (connections->count >= connections->capacity){
		
		if (connections->capacity == 0) connections->capacity = 256;
		else connections->capacity *= 2;
		
		connection* tmp = connections->items;
		connections->items = realloc(connections->items, connections->capacity * sizeof(*connections->items));
		if (!connections->items){
			printf("Error : Memory re-allocation failed. Will not store item [%d, %d]. \n", node1, node2);
			connections->items = tmp;
			return;
		}
	}
	size_t i = connections->count;
	connections->items[i].node1 = node1;
	connections->items[i].node2 = node2;

	connections->count++;
}

void CONNECTIONS_ADD_FROM_MEMORY(connections_container *connections, node* node1, node* node2){
	if(connections == NULL){
		fprintf(stderr, "Error : Skipped adding connection because nodes_container is null.\n");
		return;
	}
	if (node1 == NULL || node2 == NULL){
		fprintf(stderr, "Error, one of the nodes is NULL %p, %p", node1, node2);
		return;
	}
		
	
	if (connections->count >= connections->capacity){
		
		if (connections->capacity == 0) connections->capacity = 256;
		else connections->capacity *= 2;
		
		connection* tmp = connections->items;
		connections->items = realloc(connections->items, connections->capacity * sizeof(*connections->items));
		if (!connections->items){
			printf("Error : Memory re-allocation failed. Will not store item [%d, %d]. \n", node1, node2);
			connections->items = tmp;
			return;
		}
	}
	size_t i = connections->count;
	connections->items[i].node1 = node1->id;
	connections->items[i].node2 = node2->id;

	connections->count++;
}

connections_container* CONNECTIONS_NEW(){
	connections_container* n =  malloc(sizeof(connections_container));
	if(!n){
		fprintf(stderr, "Error : Malloc failed, couldn't create connections \n");
		return NULL;
	}
	memset(n,0, sizeof(*n));
	return n;
}



void CONNECTIONS_FREE(connections_container* connections){
	free(connections->items);
	free(connections);
}

