#include <stdio.h>
#include <stdlib.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/token.h"
#include "include/parser.h"
#include "include/ast_clean.h"

void bet_compile(char* src) {
    char* src_file = read_file(src);
    Lexer* lexer = init_lexer(src_file);

    Parser* parser = init_parser(lexer);
    parser_parse_tokens(parser);


    //clean(parser, parser->root);

    print_ast(parser);
    /*for (int i = 0; i < num_tokens; i++) {
        //printf("%s\n", token_to_string(tokens[i]));
    }*/
}
