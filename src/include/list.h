#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

typedef struct LIST_STRUCT {
    void** arr;
    size_t type_size;
    int arr_size;
    int arr_count;
} List;

List* init_list(size_t type_size);

void list_append(List* list_struct, void* data, size_t data_size);

void print_list(List* list_struct);

#endif
