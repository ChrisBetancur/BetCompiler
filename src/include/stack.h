#include "list.h"
#include "ast_node.h"
#include <stdbool.h>

#ifndef STACK_STRUCT_H
#define STACK_STRUCT_H

typedef struct STACK_STRUCT {
    List* stack_list;
    int top_offset;
} Stack;

Stack* init_stack();

void stack_push(Stack* stack_frame, ASTNode* node);

ASTNode* stack_pop(Stack* stack_frame);

ASTNode* stack_top(Stack* stack_frame);

bool stack_contains_symbol(Stack* stack_frame, ASTNode* node);

ASTNode* stack_get_symbol(Stack* stack_frame, ASTNode* node);

bool stack_is_empty(Stack* stack_frame);

void print_stack_frame(Stack* stack_frame);

#endif
