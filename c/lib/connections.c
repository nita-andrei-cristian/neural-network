#include <connections.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CONNECTIONS_ADD(connections_container *connections, long node1, long node2){
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

	connections->count++;
}

