#include "stack.h"
#include "ast_node.h"

#ifndef BET_AS_FRONTEND
#define BET_AS_FRONTEND

char* assemble(ASTNode* node, Stack* stack_frame);

char* as_op_evaluate(char* op);

char* as_eval_expr(ASTNode* node, char* reg, Stack* stack_frame);

char* as_int(ASTNode* node, Stack* stack_frame);

char* as_var_call(ASTNode* node, Stack* stack_frame);

char* as_var(ASTNode* node, Stack* stack_frame);

char* as_func(ASTNode* node);

char* as_built_in(ASTNode* node, Stack* stack_frame);

char* as_global(ASTNode* node, Stack* stack_frame);

#endif
