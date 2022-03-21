#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/parser.h"

Parser* init_parser(Lexer* lexer, int num_tokens) {
    Parser* parser = malloc(sizeof(struct PARSER_STRUCT));

    parser->root = NULL;
    parser->lexer = lexer;
    parser->num_tokens = num_tokens;
    parser->curr_token = lexer_next_token(parser->lexer);
}

void parser_eat(Parser* parser) {
    parser->curr_token = lexer_next_token(parser->lexer);
}

bool is_prim_type(char* name) {
    switch (name) {
        case strcmp(name, "int") == 0:
            return true;
            break;

        case strcmp(name, "bool") == 0:
            return true;
            break;

        case strcmp(name, "void") == 0:
            return true;
            break;

        default:
            return false;
            break;
    };
}

bool is_keyword_type(char* name) {
    switch (name) {
        case strcmp(name, "return") == 0:
            return true;
            break;

        case strcmp(name, "print") == 0:
            return true;
            break;

        default:
            return false;
            break;
    }
}

bool is_unique_symbol_name(char* name) { //future must implement to check if the symbol has already been defined in the scope
    switch (name) {
        case is_prim_type(name):
            return false;
            break;

        case is_keyword_type(name):
            return false;
            break;

    };
}

ASTNode* parse_func(Parser* parser, ASTNode* curr, Token* symbol_name) {
    if (strcmp(parser->curr_token->value, "(")) {
    
    }
}

ASTNode* parse_id(Parser* parser) {
    if (is_prim_type(parser->curr_token->name)) {
        ASTNode* curr_node = init_ASTNode(AST_TYPE, parser->curr_token->name);

        parser_eat(parser);

        if (is_unique_symbol_name(parser->curr_token->value)) {
            Token* symbol_name = parser->curr_token;
            parser_eat(parser);
            parse_func(parser, curr_node, symbol_name);
        }

    }
}

ASTNode* parser_parse(Parser* parse) {
    switch (parser->curr_token) {
        case parser->curr_token->type == TOKEN_ID:
            return parse_id(Parser* parse);
            break;
    };
}

void parser_parse_tokens(Parser* parser) {
    for (int i = 0; i < parser->num_tokens; i++) {
    }
}
