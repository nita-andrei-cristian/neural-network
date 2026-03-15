#ifndef MIN_FUNC
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef READ_FILE_FUNC
char* read_file(char* filename);
#endif

#ifndef ADD_ITEMS_FROM_DATA_FUNC
#define ADD_ITEMS_FROM_DATA_FUNC
#include "./neuro-engine/core.h"
#include <string.h>
#include <stdlib.h>

bool ADD_DATA_FROM_RESPONSE(nodes_container *nodes_data, connections_container *connections_data, char* response);
#endif

#ifndef CONVERSION_FUNCTIONS
#define CONVERSION_FUNCTIONS
void itoa(int n, char s[]);
void ltoa(long n, char s[]);
void dtoa(double n, char s[], int precision);
double atod(char s[], int precision);
#endif

#ifndef STRING_SEARCH_FUNCTIONS
#define STRING_SEARCH_FUNCTIONS

char* searchFirstDigit(char *source);

char* searchFirstDigitWithComma(char *source);

char* searchFirstNonDigitWithComma(char *source);

char* searchFirstNonDigit(char *source);

#endif
