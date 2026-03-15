#include "connections.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

connection* CONNECTIONS_ADD_FROM_IDS(connections_container *connections, nodes_container *nodes, long node1, long node2, bool decay){
	if(connections == NULL){
		fprintf(stderr, "Error : Skipped adding connection because nodes_container is null.\n");
		return NULL;
	}
	
	if (connections->count >= connections->capacity){
		
		if (connections->capacity == 0) connections->capacity = 256;
		else connections->capacity *= 2;
		
		connection* tmp = connections->items;
		connections->items = realloc(connections->items, connections->capacity * sizeof(*connections->items));
		if (!connections->items){
			printf("Error : Memory re-allocation failed. Will not store item [%d, %d]. \n", node1, node2);
			connections->items = tmp;
			return NULL;
		} 
	}

	if (decay) CONNECTIONS_DECAY(connections);

	size_t i = connections->count;
	connection *new = &connections->items[i];

	new->node1 = node1;
	new->node2 = node2;
	new->pnode1 = SEARCH_NODE_BY_ID(nodes, node1);
	new->pnode2 = SEARCH_NODE_BY_ID(nodes, node2);
	new->intensity = 1;
	new->dead = false;

	connections->count++;

	return new;
}


connection* CONNECTIONS_SEARCH_BY_NODES(connections_container *connections, long node1, long node2){

	for (int i = 0; i < connections->count; i++){
		connection target = connections->items[i];
		if (((target.node1 == node2 && target.node2 == node1) || (target.node1 == node1 && target.node2 == node2)) && !target.dead){
			return &connections->items[i];
		}
	}
	return NULL;
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

void CONNECTIONS_DECAY(connections_container *connections){
	for (int i = 0; i < connections->count; i++){
		connection *target = &connections->items[i];
		if (target->dead) continue;

		target->intensity -= 0.05;
		if (target->pnode1)
			target->pnode1->intensity -= 0.05;
		if (target->pnode2)
			target->pnode2->intensity -= 0.05;

		if (target->intensity <= 0.0){
			target->dead = true;

			if (target->pnode1->intensity <= 0)
				target->pnode1->dead = true;
			if (target->pnode2->intensity <= 0)
				target->pnode2->dead = true;

			// kill respective nodes
		}
	}
}

void CONNECTIONS_FREE(connections_container* connections){
	free(connections->items);
	free(connections);
}

