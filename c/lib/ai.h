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

void AI_SET_PATH(AI* ai, char* path);

void AI_ENSURE_ROLE(AI* ai, enum AI_ROLE role);

char* AI_FORMAT_PROMPT(AI* ai, char* user_input);

char* AI_RUN(AI* ai, char* message);

char* read_file(char* filename);

char* AI_RUN_MOCK();

AI* AI_NEW(size_t id, enum AI_ROLE role, char *path);

void AI_FREE(AI* ai);
