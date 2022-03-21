#include <stdio.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/token.h"

#include "include/stack.h"
#include "include/list.h"

int main(int argc, char* argv[]) {
    bet_compile(argv[1]);
    /*
    Stack* stack = init_stack(sizeof(int));

    for (int i = 1; i < 5; i++) {
        stack_push(stack, i, sizeof(int));
    }

    printf("Removed: %p\n", stack_pop(stack));

    stack_print(stack);*/
    /*
    List* list = init_list(sizeof(int));

    for (int i = 0; i < 10; i++) {
        list_append(list, i, sizeof(int));
    }

    print_list(list);*/
}
