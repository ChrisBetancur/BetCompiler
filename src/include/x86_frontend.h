#include "stack.h"
#include "ast_node.h"

#ifndef BET_x86_FRONTEND_H
#define BET_x86_FRONTEND_H

char* x86_assemble(ASTNode* node, Stack* stack_frame);

char* x86_built_in(ASTNode* node, Stack* stack_frame);

char* x86_op_evaluate(char* op);

char* x86_var_call(ASTNode* node, Stack* stack_frame);

char* x86_var(ASTNode* node, Stack* stack_frame);

char* x86_int(ASTNode* node);

char* x86_binary_op(ASTNode* node, Stack* stack_frame);

char* x86_eval_expr(ASTNode* node, Stack* stack_frame);

char* x86_func_call(ASTNode* node, Stack* stack_frame);

char* x86_func(ASTNode* node, Stack* stack_frame);

char* x86_global(ASTNode* node, Stack* stack_frame);

#endif
