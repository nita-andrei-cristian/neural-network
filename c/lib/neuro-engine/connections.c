#include "connections.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DecayConnections(){
	for (int i = 0; i < connections->count; i++){
		Connection *target = &connections->items[i];
		if (target->dead) continue;

		target->intensity -= 0.05;
		if (target->pnode1)
			target->pnode1->intensity -= 0.05;
		if (target->pnode2)
			target->pnode2->intensity -= 0.05;

		if (target->intensity <= 0.0){
			target->dead = 1;

			if (target->pnode1->intensity <= 0)
				target->pnode1->dead = 1;
			if (target->pnode2->intensity <= 0)
				target->pnode2->dead = 1;

			// kill respective nodes
		}
	}
}


Connection* AddConnectionFromIDs(long node1, long node2, _Bool decay){
	if(connections == NULL){
		fprintf(stderr, "Error : Skipped adding connection because nodes_container is null.\n");
		return NULL;
	}
	
	if (connections->count >= connections->capacity){
		
		if (connections->capacity == 0) connections->capacity = 256;
		else connections->capacity *= 2;
		
		Connection* tmp = connections->items;
		connections->items = realloc(connections->items, connections->capacity * sizeof(*connections->items));
		if (!connections->items){
			printf("Error : Memory re-allocation failed. Will not store item [%ld, %ld]. \n", node1, node2);
			connections->items = tmp;
			return NULL;
		} 
	}

	if (decay) DecayConnections();

	size_t i = connections->count;
	Connection *new = &connections->items[i];

	new->node1 = node1;
	new->node2 = node2;
	new->pnode1 = SearchNodeID(node1);
	new->pnode2 = SearchNodeID(node2);
	new->intensity = 1;
	new->dead = 0;

	connections->count++;

	return new;
}


Connection* findConnection(long node1, long node2){

	for (int i = 0; i < connections->count; i++){
		Connection target = connections->items[i];
		if (((target.node1 == node2 && target.node2 == node1) || (target.node1 == node1 && target.node2 == node2)) && !target.dead){
			return &connections->items[i];
		}
	}
	return NULL;
}

void init_connections(){
	free_connections();
	connections =  malloc(sizeof(ConnectionsContainer));
	if(!connections){
		fprintf(stderr, "Error : Malloc failed, couldn't create connections \n");
		return;
	}
	memset(connections, 0, sizeof(ConnectionsContainer));
}

void free_connections(){
	if (connections == NULL) return;
	if (connections->items)
		free(connections->items);
	free(connections);
}

