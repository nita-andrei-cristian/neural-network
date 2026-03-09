#include <utils.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

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
