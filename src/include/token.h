#ifndef TOKEN_H
#define TOKEN_H

/*
 * Struct: TOKEN_STRUCT
 *
 * Struct defined to represent a token
 *
 * value: represents the value of token
 * line_num: where token is located in file
 * type: represents the type of token
 */

typedef struct TOKEN_STRUCT {
    char *name;
    unsigned int line_num;

    enum {
        TOKEN_ID,
        TOKEN_EQUAL,
        TOKEN_ADD,
        TOKEN_MULT,
        TOKEN_DIV,
        TOKEN_SUB,
        TOKEN_LPARAN,
        TOKEN_RPARAN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_SEMI,
        TOKEN_COMMA,
        TOKEN_LT,
        TOKEN_GT,
        TOKEN_STRING,
        TOKEN_INT,
        TOKEN_EOL,
        TOKEN_EOF
    } type;
} token_t;

//Public functions for token struct

token_t* init_token(char *value, int type, int line_num);

char* token_type_to_string(int type);

char* token_to_string(token_t *token);

int free_token(token_t *token);

#endif
