#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

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
} list_t;

// Public functions for list

list_t* init_list(size_t type_size);

void list_append(list_t *list_struct, void* data);

void print_list(list_t *list_struct);

#endif
