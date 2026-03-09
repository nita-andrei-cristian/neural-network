#include <nodes.h>
#include <utils.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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



// note : weird edge case when one of the nodes is actually called 'connections'
bool ADD_DATA_FROM_RESPONSE(nodes_container *nodes_data, char* response){
	char *nodes = strstr(response, "\"nodes\"");
	char *connections = strstr(response, "\"connections\"");

	if (!nodes) {
		fprintf(stderr, "Error, response has no nodes param %s\n", response);
		return 0;
	}
	if (!connections) {
		fprintf(stderr, "Error, response has no connections param %s\n", response);
		return 0;
	}

	char *nodes_start = strchr(nodes, '[');
	char *nodes_end = strchr(nodes, ']');

	if (!nodes_start || !nodes_end){
		fprintf(stderr, "Error : couldn't identify nodes block");
		return 0;
	}

	char *node_start;
	char *node_end;
	
	while (true){
		node_start = strchr(nodes_start, '"');
		node_end = strchr(node_start + 1, '"');

		if (node_start >= nodes_end) break;
		if (!node_start || !node_end) break;
	
		NODES_ADD(nodes_data, node_start + 1, node_end - node_start - 1);

		nodes_start = node_end + 1;
	}
	

	char *connections_start = strchr(connections, '[');
	if (!connections_start){
		fprintf(stderr, "Error : couldn't identify nodes block");
		return 0;
	}
	
	char *token_start;
	char *token_end;
	int i = 0;
	
	while (i <= 50){
		
		if (!connections_start) break;

		token_start = strchr(connections_start + 1, '[');
		if (!token_start) break;
		token_end = strchr(token_start + 1, ']');


		if (!token_start || !token_end) break;
		if (token_start > token_end) break;

		char* first_element_start = strchr(token_start + 1, '"');
		char* first_element_end = strchr(first_element_start + 1, '"');

		char* second_element_start = strchr(first_element_end + 1, '"');
		char* second_element_end = strchr(second_element_start + 1, '"');

		if (first_element_start && first_element_end && second_element_start && second_element_end){
			char buf1[16];
			char buf2[16];
	
			memcpy(buf1, first_element_start+1, first_element_end - first_element_start - 1);
			buf1[first_element_end - first_element_start - 1]  = '\0';

			memcpy(buf2, second_element_start+1, second_element_end - second_element_start - 1);
			buf2[second_element_end - second_element_start - 1]  = '\0';

			printf("[%s] and [%s]\n", buf1, buf2);

		}

		connections_start = token_end + 1;
		i++;
	}
	
	
	return 1;
}
