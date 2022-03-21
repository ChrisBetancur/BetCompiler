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
} Parser;

Parser* init_parser(Lexer* lexer, int num_tokens);

void parser_eat(Parser* parser);

bool isPrimType(char* name);

bool isKeywordType(char* name);

ASTNode* parse_var(parser* parser);

ASTNode* parse_compound(Parser* parser);

ASTNode* parse_id(Parser* parser);

ASTNode* parser_parse(Parser* parser);

void parser_parse_tokens(Parser* parser);

#endif
