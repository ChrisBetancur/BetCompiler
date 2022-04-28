#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT {
    char* value;

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
        TOKEN_LESS,
        TOKEN_GREATER,
        TOKEN_INT,
        TOKEN_EOL,
        TOKEN_EOF
    } type;
} Token;

Token* init_token(char* value, int type);

char* token_type_to_string(int type);

char* token_to_string(Token* token);

#endif
