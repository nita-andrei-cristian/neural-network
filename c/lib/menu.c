#include <menu.h>
#include <nodes.h>
#include <utils.h>
#include <connections.h>
#include <stdio.h>

char* AI_RUN_MOCK()
{
	return read_file("/home/nita/dev/c/neural-network/mock-response.txt");
}

connections_container *connections;
nodes_container *nodes;

void MENU_START(){
	
	printf("Session Started...\n\n");

	//char path[] = "/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf";

	//AI* ai = AI_NEW(0, TUPPLE_CREATE, path);

	nodes = NODES_NEW();
	connections = CONNECTIONS_NEW();

	//AI_RUN(ai, "I hate when collegues get better grades in coding competitions, I mus tbe better to impress my parents and be able to reproduce as a human. I fear the future.");
	
	MENU_LOOP();
	MENU_END();
}

void MENU_LOOP(){
	
	char input = 'q';

	char* response = AI_RUN_MOCK();

	ADD_DATA_FROM_RESPONSE(nodes, connections, response);

	node* n = NODES_READ(nodes, 0);
	printf("Found node : %s\n", n->label);
	n = NODES_READ(nodes, 1);
	printf("Found node : %s\n", n->label);
	n = NODES_READ(nodes, 2);
	printf("Found node : %s\n", n->label);

	EXPORT_GRAPH(nodes, connections, "/home/nita/dev/c/neural-network/js/graph.json");

	if (input == 'q'){
		return;
	}

	MENU_LOOP();
}

void MENU_END(){
	printf("Session ended.\n");

	NODES_FREE(nodes);
	CONNECTIONS_FREE(connections);
	//AI_FREE(ai);
}
