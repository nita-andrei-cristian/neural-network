#include "memory.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "nodes.h"

#include "../utils.h"
#include "connections.h"

// note : weird edge case when one of the nodes is actually called 'connections'
_Bool UpdateGraphFromResponse(char* response){
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
	
	while (1){
		node_start = strchr(nodes_start, '"');
		node_end = strchr(node_start + 1, '"');
		char buf[NODE_SIZE];
		size_t len = node_end - node_start - 1;
		if (len >= 32) len = 31;
		
		memcpy(buf, node_start + 1, len);
		buf[len] = '\0';

		if (node_start >= nodes_end) break;
		if (!node_start || !node_end) break;
	
		if (!SearchNodeLabel(buf)){
			NewNode(buf, len);
		}

		nodes_start = node_end + 1;
	}
	

	char *connections_start = strchr(connections, '[');
	if (!connections_start){
		fprintf(stderr, "Error : couldn't identify nodes block");
		return 0;
	}
	
	char *token_start;
	char *token_end;
	_Bool shouldDecay = 1;
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

			Node* node1 = SearchNodeLabel(buf1);
			memcpy(buf2, second_element_start+1, second_element_end - second_element_start - 1);
			buf2[second_element_end - second_element_start - 1]  = '\0';

			Node* node2 = SearchNodeLabel(buf2);

			if (node1 && node2){
				//printf("[%p] and [%p]\n", node1, node2);
				Connection *target = findConnection(node1->id, node2->id);
				if (!target){
					AddConnectionFromIDs(node1->id, node2->id, shouldDecay);
					shouldDecay = 0;
				}else{
					target->intensity += 0.1;
					if (target->pnode1)
						target->pnode1->intensity += 0.1;
					if (target->pnode2)
						target->pnode2->intensity += 0.1;
				}
			}

		}

		connections_start = token_end + 1;
		i++;
	}
	
	
	return 1;
}

void ExportGraphTo(const char* directory){

	// shity size estimation
	int size = (nodes->count * sizeof(Node) + connections->count * sizeof(Connection)) * 1.2 + 128;
	char* output = (char*)malloc(size);
	if(output == NULL){
		fprintf(stderr, "Error : Couldn't allocate output\n");
		return;
	}
	output[0] = '\0';


	strcat(output, "{\n	\"nodes\" : [\n");
	for (int i = 0; i < nodes->count; i++){
		if (nodes->items[i].dead) continue;
		char id[16];
		ltoa(nodes->items[i].id, id); 
		
		strcat(output, "		{\"label\" : \"");
		strcat(output, nodes->items[i].label);
		strcat(output, "\", \"id\" : ");
		strcat(output, id);

		if (i != nodes->count - 1)
			strcat(output, "},\n");
		else
			strcat(output, "}\n");
	}
	strcat(output, "	],\n");

	strcat(output, "	\"connections\" : [\n");
	for (int i = 0; i < connections->count; i++){
		Connection target = connections->items[i];
		if (target.dead) continue;
		
		char id1[16], id2[16], intensity[16];
		ltoa(target.node1, id1); 
		ltoa(target.node2, id2); 
		dtoa(target.intensity, intensity, 8); 
		
		strcat(output, "		[");
		strcat(output, id1);
		strcat(output, ",");
		strcat(output, id2);
		strcat(output, ",");
		strcat(output, intensity);

		printf("Double is %s\n", intensity);

		if (i != connections->count - 1)
			strcat(output, "],\n");
		else
			strcat(output, "]\n");
	}
	strcat(output, "	]\n}\0");

	printf("%s\n", output);

	FILE *fptr;

	// Open a file in writing mode
	fptr = fopen(directory, "w");

	// Write some text to the file
	fprintf(fptr, "%s", output);

	// Close the file
	fclose(fptr); 
	
	printf("Successfully exported graph to %s\n", directory);

     	free(output);
};

_Bool SetGraph(char* path){
	char* response = readFile(path);
	
	char *nodes_str = strstr(response, "\"nodes\"");
	char *connections_str = strstr(response, "\"connections\"");

	if (!nodes_str) {
		fprintf(stderr, "Error, response has no nodes param %s\n", response);
		return 0;
	}
	if (!connections_str) {
		fprintf(stderr, "Error, response has no connections param %s\n", response);
		return 0;
	}

	char *nodes_start = strchr(nodes_str, '[');
	char *nodes_end = strchr(nodes_str, ']');

	if (!nodes_start || !nodes_end){
		fprintf(stderr, "Error : couldn't identify nodes block");
		return 0;
	}

	char *node_start;
	char *node_end;
	
	while (1){
		node_start = strchr(nodes_start, '{');
		if (!node_start) break;
		node_end = strchr(node_start + 1, '}');
		if (node_start >= nodes_end) break;
		if (!node_end) break;

		char* label_start = strstr(node_start, "\"label\"");
		char* label_end;
		if (!label_start || label_start >= node_end) continue;
		label_start = strchr(label_start + 7, '"');
		label_end = strchr(label_start + 1, '"');

		char label_data[NODE_SIZE];
		size_t len = label_end - label_start - 1;
		if (len >= 32) len = 31;

		memcpy(label_data, label_start + 1, len);
		label_data[len] = '\0';

		char* id_start = strstr(label_end, "\"id\"");
		char* id_end;
		if (!id_start || id_start >= node_end) continue;
		id_start = searchFirstDigit(id_start);
		id_end = searchFirstNonDigit(id_start+1);

		char id_data[16];
		len = id_end - id_start;
		if (len >= 16) len = 15; 

		memcpy(id_data, id_start, len);
		id_data[len] = '\0';

		if (!SearchNodeLabel(label_data)){
			Node* new = NewNode(label_data, label_end - label_start - 1);
			new->id = atol(id_data);
		}


		nodes_start = node_end + 1;
	}

	char *connections_start = strchr(connections_str, '[');
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

		char* first_element_start = searchFirstDigit(token_start);
		if(!first_element_start) break;
		char* first_element_end = searchFirstNonDigit(first_element_start);
		if(!first_element_end) break;

		char* second_element_start = searchFirstDigit(first_element_end);
		if(!second_element_start) break;
		char* second_element_end = searchFirstNonDigit(second_element_start);
		if(!second_element_end) break;

		char* third_element_start = searchFirstDigitWithComma(second_element_end);
		if(!third_element_start) break;
		char* third_element_end = searchFirstNonDigitWithComma(third_element_start);
		if(!third_element_end) break;
	
		char buf1[32];
		char buf2[32];
		char buf3[32];

		memcpy(buf1, first_element_start, first_element_end - first_element_start);
		buf1[first_element_end - first_element_start]  = '\0';
		Node* node1 = SearchNodeID(atol(buf1));

		memcpy(buf2, second_element_start, second_element_end - second_element_start);
		buf2[second_element_end - second_element_start ]  = '\0';
		Node* node2 = SearchNodeID(atol(buf2));

		memcpy(buf3, third_element_start+1, third_element_end - third_element_start - 1);
		buf3[third_element_end - third_element_start - 1 ]  = '\0';

		if (node1 && node2){
			Connection* new = AddConnectionFromIDs(node1->id, node2->id, 0);
			new->intensity = atod(buf3, strlen(buf3) - 1);
		}

		connections_start = token_end + 1;
		i++;
	}
	
	if (response) free(response);
	
	return 1;
}


