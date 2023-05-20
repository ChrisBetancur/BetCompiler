#include "ast_node.h"
#include "stack.h"

#ifndef BET_AS_BUILT_IN
#define BET_AS_BUILT_IN

char* built_in_print_var(ASTNode* call);

char* built_in_print_int(char* integer);

char* built_in_print_string(ASTNode* node);

#endif
