#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "include/lexer.h"

/*
 * Function: init_lexer
 *
 * Creates lexer to use to conver source code into tokens
 *
 * src: source code
 *
 * returns: lexer
 */

Lexer* init_lexer(char* src) {
    Lexer* lexer = calloc(1, sizeof(struct LEXER_STRUCT));

    lexer->src = src;
    lexer->src_size = (int)strlen(src);
    lexer->curr = src[0];
    lexer->index = 0;
    lexer->curr_line = 1;

    return lexer;
}

/*
 * Function: lexer_peek
 *
 * Peaks the next char of the src code
 *
 * lexer: peak char of lexer
 *
 * returns: the next char in the src code
 */

char lexer_peek(Lexer* lexer) {
    return lexer->src[lexer->index + 1];
}

/*
 * Function: lexer_advance
 *
 * Advancing lexer to the next char
 *
 * lexer: advancing lexer
 */

void lexer_advance(Lexer* lexer) {
    lexer->index++;
    lexer->curr = lexer->src[lexer->index];
}

/*
 * Function: lexer_skip_whitespace
 *
 * If curr_char is whitespace, func will skip it
 *
 * lexer: skipping whitespace in lexer
 */

void lexer_skip_whitespace(Lexer* lexer) {
    while (lexer->curr == ' ' || lexer->curr == '\t' || lexer->curr == '\n' || lexer->curr == '\0') {
        if (lexer->curr == '\n')
            lexer->curr_line++;
        lexer_advance(lexer);
    }
}

/*
 * Function: lexer_advance_string
 *
 * Advancing strings in src code
 *
 * lexer: advancing string in lexer
 *
 * returns: token with string containing the the line token is currently on
 */

Token* lexer_advance_string(Lexer* lexer) {
    char* value_buffer = calloc(1, sizeof(char));

    lexer_advance(lexer);
    while (lexer->curr != '\"') {
        value_buffer = realloc(value_buffer, (strlen(value_buffer) + 2) * sizeof(char));
        strcat(value_buffer, (char[]){lexer->curr, 0});
        lexer_advance(lexer);
    }
    lexer_advance(lexer);

    return init_token(value_buffer, TOKEN_STRING, lexer->curr_line);
}

/*
 * Function: lexer_advance_num
 *
 * Advancing number in src code
 *
 * lexer: advancing number in lexer
 *
 * returns: token with integer and current line token is on
 */

Token* lexer_advance_num(Lexer* lexer) {
    char* value_buffer = calloc(1, sizeof(char));

    while (isdigit(lexer->curr) != 0) {
        value_buffer = realloc(value_buffer, (strlen(value_buffer) + 2) * sizeof(char));
        strcat(value_buffer, (char[]){lexer->curr, 0}); // RESEARCH
        lexer_advance(lexer);
    }
    return init_token(value_buffer, TOKEN_INT, lexer->curr_line);
}

/*
 * Function: lexer_advance_once
 *
 * Advancing lexer once in src code
 *
 * lexer: advance lexer once
 * type: type of token that is being advanced
 *
 * returns: token with value and current line
 */

Token* lexer_advance_once(Lexer* lexer, int type) {

    char* token_buffer = calloc(2, sizeof(char));
    token_buffer[0] = lexer->curr;
    token_buffer[1] = '\0';
    lexer_advance(lexer);

    return init_token(token_buffer, type, lexer->curr_line);
}

/*
 * Function: lexer_advance_id
 *
 * Advancing identifier in src code
 *
 * lexer: advance id in lexer
 *
 * returns: token with id and current line
 */

Token* lexer_advance_id(Lexer* lexer) {
    char* value_buffer = calloc(1, sizeof(char));

    while (isalpha(lexer->curr) != 0 || lexer->curr == '_') {
        value_buffer = realloc(value_buffer, (strlen(value_buffer) + 2) * sizeof(char));
        strcat(value_buffer, (char[]){lexer->curr, 0}); // RESEARCH
        lexer_advance(lexer);
    }
    //printf("%s\n", value_buffer);
    return init_token(value_buffer, TOKEN_ID, lexer->curr_line);
}

/*
 * Function: lexer_next_token
 *
 * Tokenizes the next value in the src code
 *
 * lexer: getting next token in lexer
 *
 * return: token
 */

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
            case '*':
                return lexer_advance_once(lexer, TOKEN_MULT);
                break;
            case '/':
                return lexer_advance_once(lexer, TOKEN_DIV);
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
            case ',':
                return lexer_advance_once(lexer, TOKEN_COMMA);
                break;
            case '<':
                return lexer_advance_once(lexer, TOKEN_LESS);
                break;
            case '>':
                return lexer_advance_once(lexer, TOKEN_GREATER);
                break;
            case '\"':
                return lexer_advance_string(lexer);
                break;

            case ';':
                return lexer_advance_once(lexer, TOKEN_EOL);
                break;
        }
    }
    return init_token("EOF", TOKEN_EOF, lexer->curr_line);
}


