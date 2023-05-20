#include <stdio.h>
#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

/*
 * Struct: LIST_STRUCT
 *
 * Struct defined to represent a list
 *
 * arr: stores a array of values of any kind
 * data_size: size of the values stored in the array
 * num_items: number of items in the array
 */
typedef struct LIST_STRUCT {
    void** arr;
    size_t data_size;
    unsigned int num_items;
} List;

List* init_list(size_t type_size);

void list_append(List* list_struct, void* data, size_t data_size);

void print_list(List* list_struct);

#endif
