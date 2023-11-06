#ifndef LEXER_H
#define LEXER_H

#include "token.h"

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
    char *src;
    int src_size;
    char curr;
    unsigned int curr_line;
    int index;
} lexer_t;

lexer_t* init_lexer(char *src);

token_t* lexer_stream_token(lexer_t *lexer);

int free_lexer(lexer_t* lexer);

#endif
