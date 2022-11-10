#ifndef BET_AS_FRONTEND
#define BET_AS_FRONTEND
#include "ast_node.h"

char* assemble(ASTNode* node);

char* as_op_evaluate(char* op);

char* as_eval_expr(ASTNode* node, char* reg);

char* as_int(ASTNode* node);

char* as_var(ASTNode* node);

char* as_func(ASTNode* node);

char* as_global(ASTNode* node);

#endif
