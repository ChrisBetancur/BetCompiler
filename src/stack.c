#include "include/stack.h"

Stack* init_stack(size_t type_size) {
    Stack* stack = malloc(sizeof(struct STACK_STRUCT));

    stack->type_size = type_size;
    stack->head = NULL;
    stack->num_nodes = 0;

    return stack;
}

bool stack_is_empty(Stack* stack_struct) {
    return stack_struct->num_nodes == 0;
}

void stack_push(Stack* stack_struct, void* data, size_t data_size) {
    if (stack_struct->type_size != data_size) {
        printf("Stack: Incorrect Data Type for the Stack");
        return;
    }
    if (stack_struct->num_nodes == 0) {
        stack_struct->head = init_node(data, stack_struct->type_size);
        stack_struct->num_nodes++;
        return;
    }

    Node* new_node = init_node(data, stack_struct->type_size);
    stack_struct->head->next = new_node;
    new_node->prev = stack_struct->head;
    stack_struct->num_nodes++;
    
    stack_struct->head = new_node;
}

void* stack_pop(Stack* stack_struct) {
    Node* old_head = stack_struct->head;
    void* old_data = old_head->data;
    
    stack_struct->head = stack_struct->head->prev;
    stack_struct->num_nodes--;
    free(old_head);

    return old_data;
}

void stack_print(Stack* stack_struct) {
    Node* curr_node = stack_struct->head;
    while (curr_node != NULL) {
        printf("Stack: %p\n", curr_node->data);
        curr_node = curr_node->prev;
    }
}
