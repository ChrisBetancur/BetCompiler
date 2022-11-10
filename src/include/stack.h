#ifndef STACK_STRUCT_H
#define STACK_STRUCT_H
#include "list.h"

typedef struct STACK_STRUCT {
    List* list;
} Stack;

Stack* init_stack(size_t type_size);

void stack_push(List* list_struct

#endif
