#ifndef PARSER_H
#define PARSER_H

#include "node.h"
#include "token.h"
#include "lexer.h"
#include "list.h"

/*
 * Struct: PARSER_STRUCT
 *
 * Struct defined to represent a parser to translate tokens into a Abstract Syntax Tree
 *
 * root: Node defined ast the root of the AST
 * lexer: lexer used to get tokens from source code to use to put in AST
 * curr_token: current token the parser is appending to the AST
 */

typedef struct PARSER_STRUCT {
    node_t *root;
    list_t *tokens;
    lexer_t *lexer;
    token_t *curr_token;
} parser_t;

parser_t* init_parser(lexer_t *lexer);

void parser_parse_tokens(parser_t *parser);

void print_ast_at_node(node_t *node);

void print_ast(parser_t *parser);

void ast_to_file(parser_t *parser, FILE *file);

void tokens_to_file(parser_t* parser, FILE *file);

#endif
