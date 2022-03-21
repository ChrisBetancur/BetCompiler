#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT {
    char* value;

    enum {
        TOKEN_ID,
        TOKEN_EQUAL,
        TOKEN_ADD,
        TOKEN_SUB,
        TOKEN_LPARAN,
        TOKEN_RPARAN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_SEMI,
        TOKEN_COMMA,
        TOKEN_LESS,
        TOKEN_GREATER,
        TOKEN_INT,
    } type;
} Token;

Token* init_token(char* value, int type);

char* token_to_string(Token* token);

#endif
