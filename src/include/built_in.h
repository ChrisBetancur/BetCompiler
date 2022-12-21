#include "ast_node.h"
#include "stack.h"

#ifndef BET_AS_BUILT_IN
#define BET_AS_BUILT_IN

char* built_in_print(ASTNode* node, Stack* stack_frame);

char* built_in_print_int(ASTNode* node, Stack* stack_frame);

char* built_in_print_digit(ASTNode* node);

char* built_in_print_string(ASTNode* node);

#endif
