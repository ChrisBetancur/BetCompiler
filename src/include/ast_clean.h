#include <stdbool.h>
#include <ctype.h>

#include "parser.h"
#include "token.h"
#include "ast_node.h"
#include "list.h"


#ifndef AST_CLEAN_H
#define AST_CLEAN_H

bool is_symbol_declared_global(Parser* parser, char* symbol_name);

bool verify_symbol_dec(ASTNode* symbol);

bool is_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol);

bool is_symbol_declared(Parser* parser, ASTNode* symbol, ASTNode* symbol_scope);

ASTNode* get_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol);

ASTNode* get_return_val(ASTNode* curr_symbol);

bool validate_func(ASTNode* curr_func, ASTNode* ret_val);

void clean_func(Parser* parser, ASTNode* curr_symbol, ASTNode* symbol_scope, int symbol_line);

void clean_symbol(Parser* parser, ASTNode* curr_symbol, ASTNode* symbol_scope, int symbol_line);

#endif
