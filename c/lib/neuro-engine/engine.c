#include "engine.h"
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "neuro-engine/nodes.h"
#include "neuro-engine/search.h"

static char* make_mock_response(){

	char filename[128];
	unsigned int file = rand();
	file %= 4;
	file = 0;

	sprintf(filename,"/home/nita/dev/c/neural-network/mocks/action-data/%d.json",file);

	return read_file(filename);
}

struct Task *make_mock_task(){
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

static struct CmdSchema *make_schema(){
	struct CmdSchema *out = (struct CmdSchema*)malloc(sizeof(struct CmdSchema));
	for (int i = 0; i < MAX_SCHEMA_PARAMS; ++i)
		out->params[i] = NULL;

	return out;
}

static void free_schema(struct CmdSchema *in){
	if (!in) return;
	for (int i = 0; i < MAX_SCHEMA_PARAMS; ++i)
		if (in->params[i])
			free(in->params[i]);
	free(in);
}

static inline void add_context(char* context, size_t *size, char* target){
	if (target == NULL) return;

	size_t sum = strlen(context) + strlen(target);
	size_t new_capacity = *size * 2;
	if (sum >= *size){
		char* tmp = context;
		while (new_capacity <= sum){
			new_capacity *= 2;
		}
		context = (char*)realloc(context, new_capacity);
		
		if (!context){
			fprintf(stderr, "Couldn't allocate memory for context, current size : [%zu], target size : [%zu]\n", *size, new_capacity);
			context = tmp;
			return;
		}
	}
	strcat(context, target);
}

static char* run_cmd(struct CmdSchema* cmd){
	char* out = malloc(1000);
	int percentage = *(int*)cmd->params[0];
	out[0] = '\0';
	strcat(out, "Nodes : Top ");
	char buffP[16];
	itoa(percentage, buffP);

	strcat(out, buffP);
	strcat(out, "%\n");
	sprintf(out, "Top %d/ percent nodes are:", percentage);
	if (cmd->command[0] == '1' && cmd->params[0] != NULL){
		int size;
		Node** received = GET_IMPORTANT_NODES(percentage, &size);
		for (int i = 0; i < size; i++){
			char buff[32];
			dtoa(received[i]->intensity, buff, 10);
			strcat(out, received[i]->label);
			strcat(out, " : (intensity) ");
			strcat(out, buff);
			strcat(out, "\n");
		}
	}
	return out;
}

	
char* engine_start_task(struct Task *task){
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
		
	add_context(context, &context_size, task_data);
	add_context(context, &context_size, startpoint);
	add_context(context, &context_size, "\nYOUR COMMAND HISTORY:\n");

	engine_execute_step(task, context, INIT_CONTEXT_SIZE, 0);

	return context;
}

static struct CmdSchema *parse_response_json(char* json, char* errorMessage){
	// sanitize inputs
	if (json == NULL){
		strcpy(errorMessage, "Response is null!\n\0");
		return NULL;
	}

	char* cmd = strstr(json, "\"command\"");
	char* fin = strstr(json, "\"finished\"");
	if (!cmd && !fin){
		strcpy(errorMessage, "Command not found!\n");
		return NULL;
	}
	
	// process task finalization
	if (fin && strstr(fin+1, "true")){
		struct CmdSchema *out = make_schema();
		out->finished = 1;
		out->success = 1;
		return out;
	}

	// search command (1,2,3)
	char* num = searchFirstDigit(cmd + 1);
	if (!num){
		strcpy(errorMessage, "No command number found\n");
		return NULL;
	}

	struct CmdSchema *out = make_schema();

	out->success = 1;
	out->finished = 0;

	if (num[0] == '1' || num[0] == '2' || num[0] == '3'){
		memcpy(out->command, num, 1);
		out->command[1] = '\0';
	}else{
		strcpy(errorMessage, "Command is not in range, Commands are 1,2,3 + finished\n");
		return NULL;
	}

	// ---------------- PROCESS COMMANDS
	
	// CASE 1
	if (out->command[0] == '1'){
		// procentage		
		char* p = strstr(json, "percentage");
		if (!p) {
			strcpy(errorMessage, "Percentage parameter wasn't passed in command 1.\n");
			return NULL;
		}
		char* s = searchFirstDigit(p+1);
		if(!s){
			strcpy(errorMessage, "Invalid procentage number (must be 1-100)\n");
			return NULL;
		}
		char* f = searchFirstNonDigit(s+1) - 1;
		if(!f){
			strcpy(errorMessage, "Invalid procentage number (must be 1-100)\n");
			return NULL;
		}

		size_t dist = f - s;
		if (!dist) dist = 1;
		if (dist > 5) dist = 5;

		char buffer[5];
		unsigned short *n = malloc(sizeof(short));
		memcpy(buffer, s, dist);

		*n = (unsigned short) atoi(buffer);
		if (*n > 100) *n = 100;

		out->params[0] = n;
	}
	
	return out;
}

void engine_execute_step(struct Task *task, char* context, size_t context_size, unsigned short depth){

	if (depth >= task->maxDepth) return;

	char roundStr[4];
	itoa(depth, roundStr);

	add_context(context, &context_size, "\nRound ");
	add_context(context, &context_size, roundStr);
	add_context(context, &context_size, " :\n");

	// MOCK RESPONSE
	char* response = make_mock_response();
	//printf("RESPONSE GOT %s", response);
	
	// parse response
	char errorMessage[128] = "\0";
	struct CmdSchema *schema = parse_response_json(response, errorMessage);
	if (schema && schema->success){
		// add to history
		if (schema->finished){
			free_schema(schema);
			if (depth >= task->minDepth){
				add_context(context, &context_size, "Finished here");
				return;
			}else{
				add_context(context, &context_size, "AI model tried to finish here, but the task minimum depth is bigger, see above\n");
			}
		}else{
			add_context(context, &context_size, "AI model executed: ");
			add_context(context, &context_size, schema->command);
			char* out = run_cmd(schema);
			add_context(context, &context_size, "\nOutput: ");
			add_context(context, &context_size, out);
			if(out)
				free(out);
			//context_size = ADD_TO_CONTEXT(context, context_size, response);
		}
	}else{
		fprintf(stderr, "Error : %s", errorMessage);	
		add_context(context, &context_size, "encounted an error : ");
	 	add_context(context, &context_size, errorMessage);
	}

	if (response) free(response);
	free_schema(schema);

	engine_execute_step(task, context, context_size, depth + 1);
}

