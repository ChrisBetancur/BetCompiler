#include <stdbool.h>
#include <ctype.h>

#include "parser.h"
#include "token.h"
#include "ast_node.h"
#include "list.h"


#ifndef AST_CLEAN_H
#define AST_CLEAN_H

bool is_symbol_declared_global(Parser* parser, char* symbol_name);

bool is_symbol_in_scope(ASTNode* curr_symbol, ASTNode* symbol);

bool is_symbol_declared(Parser* parser, ASTNode* symbol);

ASTNode* get_return_val(ASTNode* curr_symbol);

void clean_var(Parser* parser, ASTNode* curr_symbol);

void clean(Parser* parser, ASTNode* curr_symbol);

#endif
