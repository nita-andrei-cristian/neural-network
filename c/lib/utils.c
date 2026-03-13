#include <utils.h>
#include <connections.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

char* read_file(char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f) {
		fprintf(stderr, "File %s not found.\n", filename);
		return NULL;
	};

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);

	char* buffer = malloc(size + 1);
	if (!buffer){
		fclose(f);
		fprintf(stderr, "Error allocating \"%s\" in a buffer\n", filename);
		return NULL;
	}

	fread(buffer, 1, size, f);
	buffer[size] = '\0';

	fclose(f);
	return buffer;	
}

// note : weird edge case when one of the nodes is actually called 'connections'
bool ADD_DATA_FROM_RESPONSE(nodes_container *nodes_data, connections_container *connections_data, char* response){
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
		char buf[NODE_SIZE];
		size_t len = node_end - node_start - 1;
		if (len >= 32) len = 31;
		
		memcpy(buf, node_start + 1, len);
		buf[len] = '\0';

		if (node_start >= nodes_end) break;
		if (!node_start || !node_end) break;
	
		if (!SEARCH_NODE_BY_LABEL(nodes_data, buf)){
			NODES_ADD(nodes_data, buf, len);
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
	bool shouldDecay = true;
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

			node* node1 = SEARCH_NODE_BY_LABEL(nodes_data, buf1);
			memcpy(buf2, second_element_start+1, second_element_end - second_element_start - 1);
			buf2[second_element_end - second_element_start - 1]  = '\0';

			node* node2 = SEARCH_NODE_BY_LABEL(nodes_data, buf2);

			if (node1 && node2){
				//printf("[%p] and [%p]\n", node1, node2);
				connection *target = CONNECTIONS_SEARCH_BY_NODES(connections_data, node1->id, node2->id);
				if (!target){
					CONNECTIONS_ADD_FROM_IDS(connections_data, node1->id, node2->id, shouldDecay);
					shouldDecay = false;
				}else{
					target->intensity += 0.1;
				}
			}

		}

		connections_start = token_end + 1;
		i++;
	}
	
	
	return 1;
}

void EXPORT_GRAPH(nodes_container* nodes, connections_container *connections, const char* directory){

	// shity size estimation
	int size = (nodes->count * sizeof(node) + connections->count * sizeof(connection)) * 1.2 + 128;
	char* output = (char*)malloc(size);
	if(output == NULL){
		fprintf(stderr, "Error : Couldn't allocate output\n");
		return;
	}
	output[0] = '\0';


	strcat(output, "{\n	\"nodes\" : [\n");
	for (int i = 0; i < nodes->count; i++){
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
		connection target = connections->items[i];
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
	fprintf(fptr, output);

	// Close the file
	fclose(fptr); 
	
	printf("Successfully exported graph to %s\n", directory);

     	free(output);
};


/*

itoa appeared in the first edition of
Kernighan and Ritchie's
The C Programming Language, 
on page 60.

*/

// Source - https://stackoverflow.com/a/29544416
// Posted by haccks, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-09, License - CC BY-SA 3.0
 #include <string.h>

 /* reverse:  reverse string s in place */
 void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }

}  

// Source - https://stackoverflow.com/a/29544416
// Posted by haccks, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-09, License - CC BY-SA 3.0

 /* itoa:  convert n to characters in s */
 void itoa(int n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  

 void ltoa(long n, char s[])
 {
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  

// This was findable on stack overlflow but I lost it so i head to generate with AI.

void dtoa(double n, char s[], int precision)
{
    int i = 0, sign;

    if ((sign = n < 0)) {
        n = -n;
    }

    int intPart = (int)n;
    double fracPart = n - intPart;

    /* convert integer part */
    do {
        s[i++] = intPart % 10 + '0';
    } while ((intPart /= 10) > 0);

    if (sign)
        s[i++] = '-';

    s[i] = '\0';      // IMPORTANT
    reverse(s);       // acum strlen merge corect

    i = strlen(s);    // ne mutăm la finalul stringului

    /* add decimal point */
    s[i++] = '.';

    /* convert fractional part */
    for (int j = 0; j < precision; j++) {
        fracPart *= 10;
        int digit = (int)fracPart;
        s[i++] = digit + '0';
        fracPart -= digit;
    }

    s[i] = '\0';
}
