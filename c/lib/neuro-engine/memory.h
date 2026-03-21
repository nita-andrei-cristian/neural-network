#ifndef MEMORY_MANAGEMENT_NN
#define MEMORY_MANAGEMENT_NN


// note : weird edge case when one of the nodes is actually called 'connections'
_Bool ADD_DATA_FROM_RESPONSE(char* response);

void EXPORT_GRAPH(const char* directory);

_Bool SET_INCEPTION_GRAPH(char* path);


#endif
