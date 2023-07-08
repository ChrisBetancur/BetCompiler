#include "stack.h"
#include "ast_node.h"
#include "symbol_table.h"

#ifndef BET_x86_FRONTEND_H
#define BET_x86_FRONTEND_H

char* x86_assemble(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_print_element(ASTNode* element, SymbolTable* table, Proc* proc);

char* x86_built_in(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_op_evaluate(char* op);

char* x86_var_call(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_var(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_int(ASTNode* node);

char* x86_binary_op(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_eval_expr(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_identify_literals(char* root_data, ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_identify_funcs(ASTNode* node, SymbolTable* table);

char* x86_return(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_func_block(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_func_call(ASTNode* node, SymbolTable* table, Proc* proc);

char* x86_prep_stack_frame(ASTNode* func_call_params, ASTNode* func_def_params);

char* x86_func(ASTNode* node, SymbolTable* table);

char* x86_global(ASTNode* node, SymbolTable* table);

#endif
