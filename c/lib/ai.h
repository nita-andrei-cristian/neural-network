enum AI_ROLE {
	TUPPLE_CREATE,
	CHAT,
};

typedef struct{
	size_t id;	
	size_t path_length;

	char* path;
	char* gbnf;
	char* system_input;

	size_t n_predict;
	size_t n_parallel;

	struct llama_model_params params;
	const struct llama_vocab *vocab;
	struct llama_model *model;
  	struct llama_sampler *smpl;
  	struct llama_context *ctx;


	enum AI_ROLE role;
} AI;

void AI_SET_PATH(AI* ai, char* path){
	ai->path_length = strlen(path);
	ai->path = malloc((ai->path_length + 1) * sizeof(char));
	memcpy(ai->path, path, ai->path_length * sizeof(char));
	ai->path[ai->path_length] = '\0';
	printf("Set path for [%zu] at \"%s\"\n", ai->id, ai->path);
}

void AI_ENSURE_ROLE(AI* ai, enum AI_ROLE role)
{
	char* grammar;
	char* system_input;

	switch (role) {
		case TUPPLE_CREATE:
			grammar = R"(
root ::= item+
item ::= "(" [^\r\n\x0b\x0c\x85\u2028\u2029]+ ")\n"

				)";
			system_input = R"(
		You are a piece of a mechanism, you will receive a user message, you will
		extract essential nodes from that message and connections between them. Your output will
		be in the form tupple (node a, node b), this will represent the edge of a graph. Let's consider an example:
		User says : 'I belive programming is profound and coding is poetry', analazying the user data, you will find
		that the user enjoys art and coding and philosophy probably, you will output concepts like (coding, philosophy),
		(coding, poetry), keep simple, never use plurals and produce as many as you think are coerent, try to form connections,
		in this example coding is related to both poetry and philosophy, however philosophy is necesarelly related to poetry.
		Try to understand the human needs at a fundamental level. Be carefull precise and creative. Items are very generalized,
		try to restrict each element to one word. When you are done, end the generation.
			)";
			break;
		case CHAT:
			grammar = R"(
root ::= <think> thinking </think> .*
thinking ::= !</think>*
			)";
			system_input = R"(
		your role is to find out as much external and intern info about a user. 
		external may be : job, passions, current state, internal may be :
		personality, ambitions. You will extract this info and we'll use 
		continously to deepen the internal and external profile of the user. 
		Be carefull to not overwhelm the user with too many questions and too
		diverse, keep them tight. Try to figure out also what the user wants 
		to talk about. DO NOT INVENT USER MODEL CONVERSATION, AFTER THE USER 
		RESPONSE DO ONLY ONE ANSWER. If the user repets the question, do not 
		repeat the answer!
			)";
			break;
		default:
			fprintf(stderr, "Error : AI role doesn't have a grammar\n");
			exit(EXIT_FAILURE);
	}

	{
		size_t len = strlen(grammar);
		ai->gbnf = malloc( len + 1 );
		memcpy(ai->gbnf, grammar, len);
		ai->gbnf[len] = '\0';
	}

	{
		size_t len = strlen(system_input);
		ai->system_input = malloc( len + 1 );
		memcpy(ai->system_input, system_input, len);
		ai->system_input[len] = '\0';
	}
}

char* AI_FORMAT_PROMPT(AI* ai, char* user_input)
{
	char* system_input = ai->system_input;
	const char* prompt_template = "<|im_start|>system : %s<|im_end|><|im_start|>user : %s<|im_end|><|im_start|>Model : ";

	size_t needed = snprintf(NULL, 0, prompt_template, system_input, user_input);
	char* result = malloc(needed + 1);

	if (result){
		snprintf(result, needed + 1, prompt_template, system_input, user_input);
	}else{
		fprintf(stderr, "Error : Couldn't join strings for creating a prompt");
		return NULL;
	}

	return result;
}

char* AI_RUN(AI* ai, char* message){
	char* prompt = AI_FORMAT_PROMPT(ai, message);
	char* output = (char*)malloc(256);
	size_t output_size = 0;
	size_t output_capacity = 256;


	const int n_prompt = -llama_tokenize(ai->vocab, prompt, strlen(prompt),
			NULL, 0, true, true);

	// allocate space for the tokens and tokenize the prompt
	llama_token* prompt_tokens = malloc(n_prompt * sizeof(llama_token));
	if (llama_tokenize(ai->vocab, prompt, strlen(prompt), prompt_tokens,
				n_prompt, true, true) < 0) {
		fprintf(stderr, "Error: failed to tokenize the prompt: %s\n", __func__);
		return "";
	}
	// in the cpp vesion we dinamically get prompt_tplens size instead of using n_prompt, that might cause problems

	struct llama_context_params ctx_params = llama_context_default_params();
	ctx_params.n_ctx = n_prompt + ai->n_predict + 1;
	ctx_params.n_batch = n_prompt;
	ctx_params.no_perf = false;


	ai->ctx = llama_init_from_model(ai->model, ctx_params);

	if (ai->ctx == NULL) {
		fprintf(stderr, "Error: failed to create the llama_context: %s\n",
				__func__);
		return "";
	}

	struct llama_sampler_chain_params sparams = llama_sampler_chain_default_params();
	sparams.no_perf = false;
	ai->smpl = llama_sampler_chain_init(sparams);
	// initialize the sampler
	//llama_sampler_chain_add(
	//		ai->smpl, llama_sampler_init_grammar(ai->vocab, ai->gbnf, "root"));
	llama_sampler_chain_add(ai->smpl, llama_sampler_init_greedy());
	//  llama_sampler_chain_add(smpl, llama_sampler_init_top_p(0.95, 0.05));
	// llama_sampler_chain_add(smpl, llama_sampler_init_top_k(.25));
	// llama_sampler_chain_add(smpl, llama_sampler_init_temp(0.6));
	
	llama_batch batch =
		llama_batch_get_one(prompt_tokens, n_prompt);

	if (llama_model_has_encoder(ai->model)) {
		if (llama_encode(ai->ctx, batch)) {
			fprintf(stderr, "Error : failed to eval %s\n", __func__);
			return "";
		}

		llama_token decoder_start_token_id = llama_model_decoder_start_token(ai->model);
		if (decoder_start_token_id == LLAMA_TOKEN_NULL) {
			decoder_start_token_id = llama_vocab_bos(ai->vocab);
		}

		batch = llama_batch_get_one(&decoder_start_token_id, 1);
	}

	llama_token new_token_id;

	for (int n_pos = 0; n_pos + batch.n_tokens < n_prompt + ai->n_predict;) {
		// evaluate the current batch with the transformer model
		if (llama_decode(ai->ctx, batch)) {
			fprintf(stderr, "Error : failed to eval: %s\n", __func__);
			return "";
		}

		n_pos += batch.n_tokens;

		// sample the next token
		{
			new_token_id = llama_sampler_sample(ai->smpl, ai->ctx, -1);
			// llama_sampler_accept(smpl, new_token_id);

			if (llama_vocab_is_eog(ai->vocab, new_token_id)) {
				break;
			}

			char buf[128];
			int n =
				llama_token_to_piece(ai->vocab, new_token_id, buf, sizeof(buf), 0, true);


			if (n < 0) {
				fprintf(stderr, "Error: failed to convert token to piece %s\n",
						__func__);
				return "";
			}

			output_size += n;
			if (output_size >= size_capacity - 1){
				size_capacity *= 1.5;

				char* tmp = output;
				output =  realloc(output, size_capacity);
				if (!output){
					printf("Error : Memory re-allocation failed. Will not store token %s. \n", new_token_id);
					output= tmp;
					return output;
				}

			}

			// add token to output
			strcat(output, buf);

			fwrite(buf, 1, (size_t)n, stdout);
			fflush(stdout);

			// prepare the next batch with the sampled token
			batch = llama_batch_get_one(&new_token_id, 1);
		}
	}

	printf("Output is %s\n", output);
	
	free(prompt);
	return output;
}

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

char* AI_RUN_MOCK()
{
	return read_file("/home/nita/dev/c/neural-network/mock-response.txt");
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

AI* AI_NEW(size_t id, enum AI_ROLE role, char *path)
{
	AI* ai = malloc(sizeof(AI));
	if (!ai){
		fprintf(stderr, "Error : malloc failed while allocating AI");
		return NULL;
	}
	ai->id = id;
	ai->role = role;

	AI_SET_PATH(ai, path);
	AI_ENSURE_ROLE(ai, ai->role);

	ai->params = llama_model_default_params();
	ai->params.n_gpu_layers = USEGPU;
	ai->n_predict = 512;

	ai->model = llama_model_load_from_file(path, ai->params);
	if (ai->model == NULL){
		fprintf(stderr, "Error : Unable to load model %s\n", path);
		return NULL;
	}

	ai->vocab = llama_model_get_vocab(ai->model);
	
	return ai;
}	

void AI_FREE(AI* ai)
{
	if (ai == NULL) return;
	if (ai->path != NULL)
		free(ai->path);
	free(ai);
}

