#include "menu.h"
#include "utils.h"
#include "./neuro-engine/nodes.h"
#include "./neuro-engine/connections.h"
#include "./neuro-engine/search.h"
#include "./neuro-engine/engine.h"
#include "./neuro-engine/memory.h"
#include <stdio.h>

static char* make_ai_mock()
{
	char filename[128];
	unsigned int file = rand();
	if (file < 0) file *= -1;
	file %= 5;
	
	sprintf(filename,"/home/nita/dev/c/neural-network/mocks/graph-data/%d.txt",file);
	
	return readFile(filename);
}

static inline enum INPUT_TYPE read_input(int recursive){
	char input[2];
	scanf("%c", &input[0]);

	input[1] = '\0';

	if (*input == 'q')
		return QUIT;
	if (*input == '0')
		return TEST0;
	if (*input == '1')
		return TEST1;
	if (*input == '2')
		return TEST2;
	if (*input == '3')
		return TEST3;
	if (*input == '4')
		return TEST4;
	if (*input == 'm')
		return MESSAGE;
	if (*input == 'e')
		return EXPORT;
	if (*input == 'c')
		return CLEAR;
	
	if (recursive < 100)
		return read_input(recursive++);

	return INPUT_ERROR;
}

void start_menu(){
	
	printf("Session Started...\n\n");

	//char path[] = "/home/nita/dev/cpp/change/models/qwen3-8b-q4.gguf";

	//AI* ai = AI_NEW(0, TUPPLE_CREATE, path);

	init_nodes();
	init_connections();

	//AI_RUN(ai, "I hate when collegues get better grades in coding competitions, I mus tbe better to impress my parents and be able to reproduce as a human. I fear the future.");
	
	//SET_INCEPTION_GRAPH(nodes, connections, "/home/nita/dev/c/neural-network/inception/0.json");

	tick_menu();
	end_menu();
}

void tick_menu(){

	printf("\
[m] - write message\n\
[e] - export graph\n\
[0-4] - execute test\n\
[c] - clear\n\
[q] - quit\n\
");

	enum INPUT_TYPE input = read_input(0);

	if (input == MESSAGE){
			char message[1024];
			fgets(message, 1024, stdin);
			fflush (stdin);
			message[1023] = '\0';
			printf("Sending message %s\n", message);
			
			char* response = make_ai_mock();

			UpdateGraphFromResponse(response);
	}
	if (input == EXPORT){
		printf("\033[H\033[J");
		printf("Export graph...\n\n");
		ExportGraphTo("/home/nita/dev/c/neural-network/js/graph.json");
	}
	if (input == TEST0){
		int out, i;
		Node** received = GetNodes(10, &out);
		for (i = 0; i < out; i++){
			printf("[%s], [%f]\n", received[i]->label, received[i]->intensity);
		}
		free(received);
		printf("Test 0 ran!");
	}
	if (input == TEST1){
		int out, i;
		if (nodes->count <= 0){
			printf("Please introduce a new node before running test 1\n");
			tick_menu();
			return;
		}
		printf("Searching for most important node...\n");
		double max_imp = nodes->items[0].intensity;
		Node *most_important_node = &nodes->items[0];
		for (i = 0; i < nodes->count; i++){
			if (nodes->items[i].intensity > max_imp){
				max_imp = nodes->items[i].intensity; 
				most_important_node = &nodes->items[i];
			}
		}
		printf("\n\nFound [%s]\n\n", most_important_node->label);
	
		Node** received = GetNodeNeighbours(most_important_node->id, 100, &out);
		printf("Finding neighbours...\n");
		
		for (i = 0; i < out; i++){
			printf("[%s], [%f]\n", received[i]->label, received[i]->intensity);
		}
		free(received);

		received = GetNodeNeighbours(most_important_node->id, 50, &out);
		printf("Finding top 50*/* neighbours...\n");
		
		for (i = 0; i < out; i++){
			printf("[%s], [%f]\n", received[i]->label, received[i]->intensity);
		}
		free(received);

		printf("Test 1 ran!");
	}
	if (input == TEST2){
		int out, i;
		if (nodes->count <= 0){
			printf("Please introduce a new node before running test 2\n");
			tick_menu();
			return;
		}
		printf("Searching for most important node...\n");
		double max_imp = nodes->items[0].intensity;
		Node *most_important_node = &nodes->items[0];
		for (i = 0; i < nodes->count; i++){
			if (nodes->items[i].intensity > max_imp){
				max_imp = nodes->items[i].intensity; 
				most_important_node = &nodes->items[i];
			}
		}
		printf("\n\nFound [%s]\n\n", most_important_node->label);
	
		char* received = ComputeNodeFamily(most_important_node, 100, 2, &out);
		printf("Finding neighbours...\n");
		
		printf("Result:\n%s\n", received);
		free(received);

		printf("Test 2 ran!");
	}
	if (input == TEST3){
		printf("Creating AI Task (mock)...\n");
		struct Task *task;
		task = make_mock_task();

		char* output = engine_start_task(task);

		printf("AI said:\n%s\n", output);
		
		free(task);
		free(output);

		printf("Test 3 ran!");
	}
	if (input == TEST4){
		printf("Test 4 ran!");
	}
	if (input == CLEAR){
		printf("\033[H\033[J");
	}
	if (input == QUIT || input == INPUT_ERROR){
		return;
	}

	tick_menu();
}

void end_menu(){
	printf("Session ended.\n");

	free_nodes();
	free_connections();
	//AI_FREE(ai);
}
