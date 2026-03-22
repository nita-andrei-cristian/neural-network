#ifndef MEMORY_MANAGEMENT_NN
#define MEMORY_MANAGEMENT_NN


// note : weird edge case when one of the nodes is actually called 'connections'
_Bool UpdateGraphFromResponse(char* response);

void ExportGraphTo(const char* directory);

_Bool SetGraph(char* path);


#endif
