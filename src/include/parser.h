#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ast_node.h"
#include "token.h"
#include "lexer.h"
#include "list.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct PARSER_STRUCT {
    ASTNode* root;
    Lexer* lexer;
    unsigned int num_tokens;
    Token* curr_token;
    int curr_line;
} Parser;

Parser* init_parser(Lexer* lexer);

void parser_eat(Parser* parser, int type);

bool is_prim_type(char* name);

bool is_keyword_type(char* name);

bool is_unique_symbol_name(char* name);

bool is_multdiv_op(char* name);

bool is_addsub_op(char* name);

ASTNode* parse_factor(Parser* parser);

ASTNode* parse_term(Parser* parser);

ASTNode* parse_expr(Parser* parser);

ASTNode* parse_var(Parser* parser, Token* symbol_name_token, ASTNode* def_type);

ASTNode* parse_func(Parser* parser, Token* symbol_name_token, ASTNode* def_type);

bool is_symbol_declared_global(Parser* parser, char* symbol_name);

ASTNode* parse_id(Parser* parser);

ASTNode* parser_parse(Parser* parser);

void parser_parse_tokens(Parser* parser);

int parser_count_nodes(ASTNode* node, int count);

void bfs_ast(ASTNode* root);

void traverse_ast(ASTNode* node, List* flag, int depth, bool is_last);

void print_ast(Parser* parser);

#endif
