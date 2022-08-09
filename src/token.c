#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "include/token.h"

/*
 * Function: init_token
 *
 * Creates token to store value in code to make it readable
 *
 * value: the string value from code to be stored in token
 * type: type of value that is stored in token
 * line_num: the line of file where the token is located
 *
 * returns: token ref
 */

Token* init_token(char* value, int type, int line_num) {
    Token* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    
    token->value = value;
    token->type = type;
    token->line_num = line_num;

    return token;
}

/*
 * Function: token_type_to_string
 *
 * Translates token type to string
 *
 * type: the type of token to translate
 *
 * returns: token type in string format
 */

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
        
        case TOKEN_STRING:
            return "STRING";
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

/*
 * Function: token_to_string
 *
 * Translates token to readable string
 *
 * token: token used to translate
 *
 * returns: token in string format
 */

char* token_to_string(Token* token) {
    const char* type_str = token_type_to_string(token->type);
    const char* template = "Token <value: %s type: %s>\n";
    char* token_str = calloc(strlen(type_str) + strlen(template) + strlen(token->value), sizeof(char));
    sprintf(token_str, template, token->value, type_str);

    return token_str;
}
