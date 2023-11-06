#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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

token_t* init_token(char *name, int type, int line_num) {
    token_t* token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->name = name;
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
        case TOKEN_LT:
            return "<";
            break;
        case TOKEN_GT:
            return ">";
            break;
        case TOKEN_STRING:
            return "STRING";
            break;

        case TOKEN_INT:
            return "INT";
            break;

        case TOKEN_EOL:
            return ";";
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

char* token_to_string(token_t *token) {
    const char *type_str = token_type_to_string(token->type);
    const char *template = "Token <value: %s type: %s line: %s>\n";

    int length = snprintf(NULL, 0, "%d", token->line_num);
    char line_num[length];
    snprintf(line_num, length + 1, "%d", token->line_num);

    char *token_str = calloc(strlen(type_str) + strlen(template) + strlen(token->name) + strlen(line_num) + 1, sizeof(char));
    sprintf(token_str, template, token->name, type_str, line_num);

    return token_str;
}


/*
 * Function: free_token
 *
 * Frees token pointer from memory
 *
 * token: token pointer to be freed
 *
 * returns: 0 if succesfully freed otherwise 1
 */

int free_token(token_t *token) {
    if (token == NULL)
        return 1;

    free(token->name);
    free(token);

    return 0;
}
