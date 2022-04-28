#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef STACK_H
#define STACK_H

typedef struct STACK_STRUCT {
    size_t type_size;
    int num_nodes;
    Node* head;
} Stack;

Stack* init_stack(size_t type_size);

bool stack_is_empty(Stack* stack_struct);

void stack_push_first(Stack* stack_struct, void* data, size_t data_size);

void* stack_pop_first(Stack* stack_struct);

void stack_push(Stack* stack_struct, void* data, size_t data_size);

void* stack_pop(Stack* stack_struct);

void stack_print(Stack* stack_struct);

#endif
