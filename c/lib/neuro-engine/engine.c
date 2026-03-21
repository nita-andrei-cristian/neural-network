#include "engine.h"
#include <string.h>
#include "../utils.h"

static char* CREATE_MOCK_RESPONSE(){

	char filename[128];
	unsigned int file = rand();
	file %= 4;

	sprintf(filename,"/home/nita/dev/c/neural-network/mocks/action-data/%d.json",file);

	return read_file(filename);
}

static void free_schema(struct CmdSchema *in){
	if (!in) return;
	for (int i = 0; i < MAX_SCHEMA_PARAMS; ++i)
		if (in->params[i])
			free(in->params[i]);
	free(in);
}

static inline size_t ADD_TO_CONTEXT(char* context, size_t size, char* target){
	if (target == NULL) return size;

	size_t sum = strlen(context) + strlen(target);
	size_t new_capacity = size * 2;
	if (sum >= size){
		char* tmp = context;
		while (new_capacity <= sum){
			new_capacity *= 2;
		}
		context = (char*)realloc(context, new_capacity);
		
		if (!context){
			fprintf(stderr, "Couldn't allocate memory for context, current size : [%zu], target size : [%zu]\n", size, new_capacity);
			context = tmp;
			return size;
		}
	}
	strcat(context, target);
	return new_capacity;
}
	
char* ENGINE_BEGIN_TASK(struct Task *task, nodes_container *nodes, connections_container *connections){
	size_t context_size = INIT_CONTEXT_SIZE;
	char* context = (char*)malloc(context_size);	
	if (!context){
		fprintf(stderr, "Error : Couldn't allocate memory for task");
		return NULL;
	}

	context[0] = '\0';

	char startpoint[] = INIT_PROMPT;

	char task_data[128] = "TASK DATA:\n";
	strcat(task_data, task->payload);

	char minDepth[4], maxDepth[4], nNodes[16], nConnections[16];
	itoa(task->minDepth, minDepth);
	itoa(task->maxDepth, maxDepth);
	itoa(nodes->count, nNodes);
	itoa(connections->count, nConnections);

	strcat(task_data, "\nMin depth: ");
	strcat(task_data, minDepth);
	strcat(task_data, "\nMax depth");
	strcat(task_data, maxDepth);
	strcat(task_data, "\nGraph contains ");
	strcat(task_data, nNodes);
	strcat(task_data, " nodes and ");
	strcat(task_data, nConnections);
	strcat(task_data, " connections.\n\n");
		
	context_size = ADD_TO_CONTEXT(context, context_size, task_data);
	context_size = ADD_TO_CONTEXT(context, context_size, startpoint);

	context_size = ADD_TO_CONTEXT(context, context_size, "\nYOUR COMMAND HISTORY:\n");

	// 0.5 Create mock responses
	
	// 1. send message to AI

	// 1.5 process commands (1,2,3)

	// 2. detect error and redirect to AI (including new limit off course)

	ENGINE_EXECUTE_STEP(task, context, INIT_CONTEXT_SIZE, 0);

	return context;
}

static struct CmdSchema *PARSE_JSON_RESPONSE(char* response, char* errorMessage){
	if (response == NULL){
		strcpy(errorMessage, "Response is null!\n\0");
		return NULL;
	}

	printf("Res len %zu\n", strlen(response));

	char* cmd = strstr(response, "\"command\"");
	char* fin = strstr(response, "\"finished\"");
	if (!cmd && !fin){
		strcpy(errorMessage, "Command not found!\n");
		return NULL;
	}
	if (fin && strstr(fin+1, "true")){
		printf("Finished mock\n");
		struct CmdSchema *out = (struct CmdSchema*)malloc(sizeof(struct CmdSchema));
		out->finished = 1;
		out->success = 1;
		return out;
	}

	char* num = searchFirstDigit(cmd + 1);
	if (!num){
		strcpy(errorMessage, "No command number found\n");
		return NULL;
	}

	struct CmdSchema *out = (struct CmdSchema*)malloc(sizeof(struct CmdSchema));

	out->success = 1;
	out->finished = 0;

	if (num[0] == '1' || num[0] == '2' || num[0] == '3'){
		memcpy(out->command, num, 1);
		out->command[1] = '\0';
	}else{
		strcpy(errorMessage, "Command is not in range, edit if you support more commands than 1,2,3\n");
		return NULL;
	}
	
	return out;
}

void ENGINE_EXECUTE_STEP(struct Task *task, char* context, size_t context_size, unsigned short depth){

	if (depth >= task->maxDepth) return;

	char roundStr[4];
	itoa(depth, roundStr);

	context_size = ADD_TO_CONTEXT(context, context_size, "\nRound ");
	context_size = ADD_TO_CONTEXT(context, context_size, roundStr);
	context_size = ADD_TO_CONTEXT(context, context_size, " :\n");

	// MOCK RESPONSE
	char* response = CREATE_MOCK_RESPONSE();
	//printf("RESPONSE GOT %s", response);
	
	// parse response
	char errorMessage[128] = "\0";
	struct CmdSchema *schema = PARSE_JSON_RESPONSE(response, errorMessage);
	
	if (schema && schema->success){
		// add to history
		if (schema->finished){
			free_schema(schema);
			if (depth >= task->minDepth){
				context_size = ADD_TO_CONTEXT(context, context_size, "Finished here");
				return;
			}else{
				context_size = ADD_TO_CONTEXT(context, context_size, "AI model tried to finish here, but the task minimum depth is bigger, see above\n");
			}
		}else{
			context_size = ADD_TO_CONTEXT(context, context_size, "AI model executed: ");
			context_size = ADD_TO_CONTEXT(context, context_size, schema->command);
			//context_size = ADD_TO_CONTEXT(context, context_size, response);
		}
	}else{
		fprintf(stderr, "Error : %s", errorMessage);	
		context_size = ADD_TO_CONTEXT(context, context_size, "encounted an error : ");
		context_size = ADD_TO_CONTEXT(context, context_size, errorMessage);
	}

	if (response) free(response);
	free_schema(schema);

	ENGINE_EXECUTE_STEP(task, context, context_size, depth + 1);
}

struct Task *CREATE_MOCK_TASK(){
	char *mock = "Create a user mission!\n\0";
	struct Task *task = (struct Task*)malloc(sizeof(struct Task) + strlen(mock));

	task->payload = (char*)malloc(strlen(mock));
	memcpy(task->payload, mock, strlen(mock));
	task->minDepth = 7;
	task->maxDepth = 10;

	printf("Task was generated\n");
	printf("%s\n", task->payload);

	return task;
}
