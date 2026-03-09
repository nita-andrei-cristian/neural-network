#include <stdio.h>

#include <nodes.h>
#include <utils.h>
#include <connections.h>

char* AI_RUN_MOCK()
{
	return read_file("/home/nita/dev/c/neural-network/mock-response.txt");
}

int main(){
	//char path[] = "/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf";

	//AI* ai = AI_NEW(0, TUPPLE_CREATE, path);

	nodes_container *nodes = NODES_NEW();

	//AI_RUN(ai, "I hate when collegues get better grades in coding competitions, I mus tbe better to impress my parents and be able to reproduce as a human. I fear the future.");
	char* response = AI_RUN_MOCK();

	ADD_DATA_FROM_RESPONSE(nodes, response);

	node* n = NODES_READ(nodes, 0);
	printf("Found node : %s\n", n->label);
	n = NODES_READ(nodes, 1);
	printf("Found node : %s\n", n->label);
	n = NODES_READ(nodes, 2);
	printf("Found node : %s\n", n->label);
	
	NODES_FREE(nodes);
	//AI_FREE(ai);

	return 0;
}
