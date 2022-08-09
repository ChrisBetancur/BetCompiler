#include "token.h"

#ifndef LEXER_H
#define LEXER_H

/*
 * Struct: LEXER_STRUCT
 *
 * Struct defined to represents a lexer
 *
 * src: contains the source code to translate into tokens
 * src_size: size of the src code
 * curr: curr char that the lexer will tokeize
 * curr_line: current line that the lexer is tokenizing
 */
typedef struct LEXER_STRUCT {
    char* src;
    int src_size;
    char curr;
    unsigned int curr_line;
    int index;
} Lexer;

Lexer* init_lexer(char* src);

char lexer_peek(Lexer* lexer);

void lexer_advance(Lexer* lexer);

void lexer_skip_whitespace(Lexer* lexer);

Token* lexer_advance_string(Lexer* lexer);

Token* lexer_advance_num(Lexer* lexer);

Token* lexer_advance_once(Lexer* lexer, int type);

Token* lexer_advance_id(Lexer* lexer);

int lexer_resize_token_arr(Token*** tokens, int index, int size);

Token* lexer_next_token(Lexer* lexer);

#endif
