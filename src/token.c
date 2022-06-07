#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/token.h"

Token* init_token(char* value, int type) {
    Token* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    
    token->value = value;
    token->type = type;

    return token;
}

char* token_type_to_string(int type) {
    switch (type) {
        case TOKEN_ID:
            return "ID";
            break;
        case TOKEN_EQUAL:
            return "=";
            break;
        case TOKEN_ADD:
            return "+";
            break;
        case TOKEN_SUB:
            return "-";
            break;
        case TOKEN_MULT:
            return "*";
            break;
        case TOKEN_DIV:
            return "/";
            break;
        case TOKEN_LPARAN:
            return "(";
            break;
        case TOKEN_RPARAN:
            return ")";
            break;
        case TOKEN_LBRACE:
            return "{";
            break;
        case TOKEN_RBRACE:
            return "}";
            break;
        case TOKEN_COMMA:
            return ",";
            break;
        case TOKEN_LESS:
            return "<";
            break;
 
        case TOKEN_GREATER:
            return ">";
            break;
        case TOKEN_INT:
            return "INT";
            break;

        case TOKEN_EOL:
            return "';' EOL";
            break;
 
        case TOKEN_EOF:
            return "EOF";
            break;
 
        default:
            return "ERROR: TOKEN TYPE NOT STRINGABLE";
            break;
    }

}

char* token_to_string(Token* token) {
    const char* type_str = token_type_to_string(token->type);
    const char* template = "Token <value: %s type: %s>\n";
    char* token_str = calloc(strlen(type_str) + strlen(template) + strlen(token->value), sizeof(char));
    sprintf(token_str, template, token->value, type_str);

    return token_str;
}
