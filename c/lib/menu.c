#include <menu.h>
#include <nodes.h>
#include <utils.h>
#include <connections.h>
#include <stdio.h>

char* AI_RUN_MOCK()
{
	char filename[128];
	int file = rand();
	if (file < 0) file *= -1;
	file %= 5;
	
	sprintf(filename,"/home/nita/dev/c/neural-network/mocks/%d.txt",file);
	
	return read_file(filename);
}

enum INPUT_TYPE PROCESS_INPUT(int recursive){
	char input[2];
	scanf("%c", &input);

	input[1] = '\0';

	if (*input == 'q')
		return QUIT;
	if (*input == 't')
		return TEST;
	if (*input == 'm')
		return MESSAGE;
	if (*input == 'e')
		return EXPORT;
	if (*input == 'c')
		return CLEAR;
	
	if (recursive < 100)
		return PROCESS_INPUT(recursive++);

	return INPUT_ERROR;
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

	printf(R"(
[m] - write message
[e] - export graph
[t] - execute test
[c] - clear
[q] - quit
)");

	enum INPUT_TYPE input = PROCESS_INPUT(0);

	if (input == MESSAGE){
			char message[1024];
			fgets(message, 1024, stdin);
			fflush (stdin);
			message[1023] = '\0';
			printf("Sending message %s\n", message);
			
			char* response = AI_RUN_MOCK();

			ADD_DATA_FROM_RESPONSE(nodes, connections, response);
			node* n = NODES_READ(nodes, 0);
			//printf("Found node : %s\n", n->label);
	}
	if (input == EXPORT){
		printf("\033[H\033[J");
		printf("Export graph...\n\n");
		EXPORT_GRAPH(nodes, connections, "/home/nita/dev/c/neural-network/js/graph.json");
	}
	if (input == TEST){
		printf("Test ran!");
	}
	if (input == CLEAR){
		printf("\033[H\033[J");
	}
	if (input == QUIT || input == INPUT_ERROR){
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
