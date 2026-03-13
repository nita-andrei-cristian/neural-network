#ifndef MIN_FUNC
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef READ_FILE_FUNC
char* read_file(char* filename);
#endif

#ifndef ADD_ITEMS_FROM_DATA_FUNC
#define ADD_ITEMS_FROM_DATA_FUNC
#include <nodes.h>
#include <connections.h>
#include <string.h>
#include <stdlib.h>

bool ADD_DATA_FROM_RESPONSE(nodes_container *nodes_data, connections_container *connections_data, char* response);
#endif

#ifndef EXPORT_GRAPH_FUNC
#define EXPORT_GRAPH_FUNC
void EXPORT_GRAPH(nodes_container* nodes, connections_container *connections_data, const char* directory);
#endif

#ifndef INCEPTION_GRAPH
#define INCEPTION_GRAPH
bool SET_INCEPTION_GRAPH(nodes_container *nodes_data, connections_container *connections_data, char* path);
#endif

#ifndef CONVERSION_FUNCTIONS
#define CONVERSION_FUNCTIONS
void itoa(int n, char s[]);
void ltoa(long n, char s[]);
void dtoa(double n, char s[], int precision);
double atod(char s[], int precision);
#endif
