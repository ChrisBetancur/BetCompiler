#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "include/lexer.h"

Lexer* init_lexer(char* src) {
    Lexer* lexer = calloc(1, sizeof(struct LEXER_STRUCT));

    lexer->src = src;
    lexer->src_size = (int)strlen(src);
    lexer->curr = src[0];
    lexer->index = 0;

    return lexer;
}

char lexer_peek(Lexer* lexer) {
    return lexer->src[lexer->index + 1];
}

void lexer_advance(Lexer* lexer) {
    lexer->index++;
    lexer->curr = lexer->src[lexer->index];
}

void lexer_skip_whitespace(Lexer* lexer) {
    while (lexer->curr == ' ' || lexer->curr == '\t' || lexer->curr == '\n' || lexer->curr == '\0') {
        lexer_advance(lexer);
    }
}

Token* lexer_advance_num(Lexer* lexer) {
    char* value_buffer = calloc(1, sizeof(char));
    
    while (isdigit(lexer->curr) != 0) {
        value_buffer = realloc(value_buffer, (strlen(value_buffer) + 2) * sizeof(char));
        strcat(value_buffer, (char[]){lexer->curr, 0}); // RESEARCH
        lexer_advance(lexer);
    }
    return init_token(value_buffer, TOKEN_INT);
}

Token* lexer_advance_once(Lexer* lexer, int type) {
    
    char* token_buffer = calloc(2, sizeof(char));
    token_buffer[0] = lexer->curr;
    token_buffer[1] = '\0';

    lexer_advance(lexer);

    return init_token(token_buffer, type);
}

Token* lexer_advance_id(Lexer* lexer) {
    char* value_buffer = calloc(1, sizeof(char));
    
    while (isalpha(lexer->curr) != 0 || lexer->curr == '_') {
        value_buffer = realloc(value_buffer, (strlen(value_buffer) + 2) * sizeof(char));
        strcat(value_buffer, (char[]){lexer->curr, 0}); // RESEARCH
        lexer_advance(lexer);
    }
    //printf("%s\n", value_buffer);
    return init_token(value_buffer, TOKEN_ID);
}

Token* lexer_next_token(Lexer* lexer) {
    while (lexer->index < lexer->src_size) {
        if (isalpha(lexer->curr) != 0) {
            return lexer_advance_id(lexer);
        }
        
        if (isdigit(lexer->curr) != 0) {
            return lexer_advance_num(lexer);
        }
        
        lexer_skip_whitespace(lexer);
        switch (lexer->curr) {
            case '=':
                return lexer_advance_once(lexer, TOKEN_EQUAL);
                break;
            case '+':
                return lexer_advance_once(lexer, TOKEN_ADD);
                break;
            case '-':
                return lexer_advance_once(lexer, TOKEN_SUB);
                break;
            case '(':
                return lexer_advance_once(lexer, TOKEN_LPARAN);
                break;
            case ')':
                return lexer_advance_once(lexer, TOKEN_RPARAN);
                break;
            case '{':
                return lexer_advance_once(lexer, TOKEN_LBRACE);
                break;
            case '}':
                return lexer_advance_once(lexer, TOKEN_RBRACE);
                break;
            case ';':
                return lexer_advance_once(lexer, TOKEN_SEMI);
                break;
            case ',':
                return lexer_advance_once(lexer, TOKEN_COMMA);
                break;
            case '<':
                return lexer_advance_once(lexer, TOKEN_LESS);
                break;
            case '>':
                return lexer_advance_once(lexer, TOKEN_GREATER);
                break;

        }
    }
    return NULL;
}


