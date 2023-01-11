#include "include/stack.h"

#include <string.h>

Stack* init_stack() {
    Stack* stack = calloc(1, sizeof(struct STACK_STRUCT));

    stack->stack_list = init_list(sizeof(struct AST_NODE_STRUCT));
    stack->top_offset = 0x4;
    return stack;
}

void stack_push(Stack* stack_frame, ASTNode* node) {
    node->offset = stack_frame->top_offset;
    stack_frame->top_offset += 0x4;

    list_append(stack_frame->stack_list, node, sizeof(struct AST_NODE_STRUCT));
}

ASTNode* stack_pop(Stack* stack_frame) {
    ASTNode* top = stack_frame->stack_list->arr[stack_frame->stack_list->num_items - 1];

    free(stack_frame->stack_list->arr[stack_frame->stack_list->num_items - 1]);
    stack_frame->stack_list->num_items--;
    realloc(stack_frame->stack_list->arr, stack_frame->stack_list->num_items * sizeof(struct STACK_STRUCT)); // IDK WHY IM GETTING ERROR HERE

    return top;
}

ASTNode* stack_top(Stack* stack_frame) {
    return stack_frame->stack_list->arr[stack_frame->stack_list->num_items - 1];
}

bool stack_contains_symbol(Stack* stack_frame, ASTNode* node) {
    for (int i = stack_frame->stack_list->num_items - 1; i >= 0; i--) {
        ASTNode* curr_node = stack_frame->stack_list->arr[i];

        if (strcmp(curr_node->name, node->name) == 0 && curr_node->type == node->type) {
            return true;
        }
    }

    return false;
}

ASTNode* stack_get_symbol(Stack* stack_frame, ASTNode* node) {
    for (int i = stack_frame->stack_list->num_items - 1; i >= 0; i--) {
        ASTNode* curr_node = stack_frame->stack_list->arr[i];

        if (strcmp(curr_node->name, node->name) == 0 && curr_node->type == node->type) {
            return curr_node;
        }
    }

    return NULL;
}

bool stack_is_empty(Stack* stack_frame) {
    return stack_frame->stack_list->num_items == 0;
}

void print_stack_frame(Stack* stack_frame) {
    printf("--Stack--\n");

    for (int i = stack_frame->stack_list->num_items - 1; i >= 0; i--) {
        printf("%s :: %c\n\n", astnode_to_string(stack_frame->stack_list->arr[i]), ((ASTNode*)stack_frame->stack_list->arr[i])->offset);
    }
}
