#include "search.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline void swap_double(double *a, double *b)
{
    double t = *a;
    *a = *b;
    *b = t;
}

// AI GENERATED CODE FOR QUICK_SELECT
static int partition_desc_double(double arr[], int left, int right)
{ double pivot = arr[right];
    int store = left;

    for (int i = left; i < right; i++) {
        if (arr[i] > pivot) {
            swap_double(&arr[i], &arr[store]);
            store++;
        }
    }

    swap_double(&arr[store], &arr[right]);
    return store;
}

static double quickselect_desc_double(double arr[], int count, int k)
{
    int left = 0;
    int right = count - 1;

    while (left <= right) {
        int pivot_index = partition_desc_double(arr, left, right);

        if (pivot_index == k)
            return arr[pivot_index];

        if (k < pivot_index)
            right = pivot_index - 1;
        else
            left = pivot_index + 1;
    }

    return arr[k];
}

static inline int top_count_percent(int total, int percent)
{
    return (total * percent + 99) / 100;
}

node **GET_IMPORTANT_NODES(nodes_container *nodes, int percent, int *out_count){
	if (out_count)
		*out_count = 0;

	if (nodes == NULL || nodes->items == NULL || nodes->count <= 0)
		return NULL;

	if (percent <= 0)
		return NULL;

	if (percent > 100)
		percent = 100;

	int n = nodes->count;
	int top_n = top_count_percent(n, percent);
	int k = top_n - 1;

	double *values = malloc((size_t)n * sizeof(*values));
	if (values == NULL)
		return NULL;

	for (int i = 0; i < n; i++)
		values[i] = nodes->items[i].intensity;

	double threshold = quickselect_desc_double(values, n, k);

	free(values);

	node **result = malloc((size_t)n * sizeof(*result));
	if (result == NULL)
		return NULL;

	int count = 0;
	for (int i = 0; i < n; i++) {
		if (nodes->items[i].intensity >= threshold) {
			result[count++] = &nodes->items[i];
		}
	}

	if (out_count)
		*out_count = count;

	return result;
}

static node** GET_NEIGHBOURS(connections_container *connections, long node_id, int *count)
{
	if (connections == NULL) return NULL;
	
	int i, lastIndex;
	node **found;

	found = (node**)malloc(100 * sizeof(node*)); // max 100 items

	lastIndex = 0;
	for (i = 0; i < connections->count && lastIndex < 100; i++){
		connection* target = &connections->items[i];
		if (target->node1 == node_id && target->pnode2 != NULL){
			found[lastIndex++] = target->pnode2;
		}else if(target->node2 == node_id && target->pnode1 != NULL){
			found[lastIndex++] = target->pnode1;
		}
	}

	if (lastIndex >= 100) lastIndex = 99;
	*count = lastIndex;

	return (node**)found;
}

node** GET_IMPORTANT_NEIGHBOURS(connections_container *connections, long node_id, int percent, int *count){
	node** found;

	if (count)
		*count = 0;

	if (connections == NULL || connections->items == NULL || connections->count <= 0)
		return NULL;

	if (percent <= 0)
		return NULL;

	if (percent > 100)
		percent = 100;

	found = GET_NEIGHBOURS(connections, node_id, count);

	int n = *count;
	int top_n = top_count_percent(n, percent);
	int k = top_n - 1;

	if (count == 0) return NULL;

	double *values = malloc((size_t)n * sizeof(*values));
	if (values == NULL)
		return NULL;

	for (int i = 0; i < n; i++)
		values[i] = found[i]->intensity;

	double threshold = quickselect_desc_double(values, n, k);

	printf("Identified treshhold : %f\n\n", threshold);

	free(values);

	// filtered neighbours
	node **result = malloc((size_t)n * sizeof(*result));
	if (result == NULL)
		return NULL;

	*count = 0;
	for (int i = 0; i < n; i++) {
		if (found[i]->intensity >= threshold) {
			printf("Node %s is added..\n", found[i]->label);
			result[(*count)++] = found[i];
		}
	}
	*count--;

	return found;
}

static char* recursive_call_neighbours(connections_container *connections, char* result, node* root, int percent, int depth){
	int count;
	node** found;
	found = GET_IMPORTANT_NEIGHBOURS(connections, root->id, percent, &count);

	strcat(result, "[Name: \"");
	strcat(result, root->label);
	strcat(result, "\",Children : [");
	if (depth > 1) strcat(result, "\n");

	for (int i = 0; i < count; i++){
		if (depth == 1){
			strcat(result, "\"");
			strcat(result, found[i]->label);
			strcat(result, "\"");
		}
		else{
			recursive_call_neighbours(connections, result, found[i], percent, depth-1);
		}
		strcat(result, ", ");
	}
	if (depth > 1) strcat(result, "\n");
	strcat(result, "]\n]");

	free(found);
}

char* COMPUTE_IMPORTANT_NEIGHBOURS_RECURSIVE(connections_container *connections, node *root, int percent, int recursive, int *count){
	
	if (recursive > 5) recursive = 5;
	if (recursive < 1) recursive = 1;
	
	if (root == NULL)
		return NULL;
	if (count)
		*count = 0;

	if (connections == NULL || connections->items == NULL || connections->count <= 0)
		return NULL;

	if (percent <= 0)
		return NULL;

	if (percent > 100)
		percent = 100;

	char *result = (char*)malloc(1024);
	result[0] = '\0';

	recursive_call_neighbours(connections, result, root, percent, recursive);

	return (char*)result;
}
