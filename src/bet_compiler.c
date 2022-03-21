#include <stdio.h>
#include <stdlib.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/token.h"

void bet_compile(char* src) {
    char* src_file = read_file(src);
    Lexer* lexer = init_lexer(src_file);

    int num_tokens = 0;
    Token** tokens = malloc(sizeof(struct TOKEN_STRUCT));

    while (lexer->index < lexer->src_size) {
        Token* token = lexer_next_token(lexer);

        //printf("%s\n", token_to_string(token));

        if (token != NULL)
            tokens[num_tokens] = token;
        else
            break;

        num_tokens++;
        tokens = realloc(tokens, (num_tokens + 1) * sizeof(struct TOKEN_STRUCT));
    }

    for (int i = 0; i < num_tokens; i++) {
        printf("%s\n", token_to_string(tokens[i]));
    }
}
