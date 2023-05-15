#include <stdio.h>

#include "include/bet_compiler.h"
#include "include/ast_node.h"
#include "include/stack.h"

int main(int argc, char* argv[]) {
    //bet_compile(argv[1]);
    bet_ide_compile(argv[1]);

    /*Stack* stack = init_stack();

    ASTNode* node = init_ASTNode("hey", AST_VAR);
    printf("%p\n", node);
    free(node);
    printf("%d\n", node == NULL);
    printf("%p\n", node);
    ASTNode* node2 = init_ASTNode("hey bro", AST_VAR);
    ASTNode* node3 = init_ASTNode("hey broski", AST_VAR);

    stack_push(stack, node);
    stack_push(stack, node2);
    stack_push(stack, node3);

    printf("%d %d\n", true, stack_contains_symbol(stack, init_ASTNode("hey bro", AST_VAR)));

    print_stack_frame(stack);

    printf("%s\n", astnode_to_string(stack_pop(stack)));

    print_stack_frame(stack);*/
}
