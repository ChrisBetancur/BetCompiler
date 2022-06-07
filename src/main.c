#include <stdio.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/token.h"
#include "include/ast_node.h"

#include "include/stack.h"
#include "include/list.h"

int main(int argc, char* argv[]) {
    bet_compile(argv[1]);
    /* 
    Stack* stack = init_stack(sizeof(int));

    for (int i = 1; i < 2; i++) {
        stack_push(stack, i, sizeof(int));
    }
    stack_print(stack);
    printf("Head: %p\n", stack->head->data);
    printf("Removed: %p\n", stack_pop_first(stack));
    
    stack_print(stack);
    */
    //List* list = init_list(sizeof(int));
    /* 
    for (int i = 0; i < 10; i++) {
        printf("Hey there\n");
        list_append(list, i, sizeof(int));
    }
    print_list(list);
    */
    //list_append(list, init_ASTNode(AST_INT, "bro"), sizeof(struct AST_NODE_STRUCT));
    /* 
    Value* value = malloc(sizeof(struct val));
    value->t = 1;
    
    Value* value2 = malloc(sizeof(struct val));
    value2->t = 4;

    Value* value3 = malloc(sizeof(struct val));
    value3->t = 9;
 
    list_append(list, value, sizeof(struct val));
    list_append(list, value2, sizeof(struct val));
    list_append(list, value3, sizeof(struct val));
 
    for (int i = 0; i < 10; i++) {
        Value* value = malloc(sizeof(struct val));
        list_append(list, value, sizeof(struct val));
        //printf("Bro %d\n", value->t);
        //free(value);
        //value = NULL;
        //value = realloc(value, sizeof(struct val));
        //value->t = i;
    }

    for (int i = 0; i < 10; i++) {
        ((Value*)list->arr[i])->t = i;
    }

    print(list);
    //printf("%d\n", ((Value*)list->arr[7])->t);
    */
}
