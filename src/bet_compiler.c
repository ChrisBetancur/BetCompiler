#include <stdio.h>
#include <stdlib.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/token.h"
#include "include/parser.h"
#include "include/ast_clean.h"
#include "include/as_frontend.h"

#define COMPILE_OBJ "as output.s -o output.o"
#define MAKE_EXECUTABLE "ld output.o -o output -macosx_version_min 11.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem"
#define RUN_EXEC "./output"

/*
 * Function: bet_compile
 *
 * Takes the file contents and compiles
 *  1. Translates code to tokens
 *  2. Translates tokens to Abstract Syntax Tree
 */

void bet_compile(char* src) {
    char* src_file = read_file(src);
    Lexer* lexer = init_lexer(src_file);

    Parser* parser = init_parser(lexer);
    parser_parse_tokens(parser);

    print_ast(parser);

    char* output = assemble(parser->root);

    write_file("output.s", output);
    sh(COMPILE_OBJ);
    sh(MAKE_EXECUTABLE);
    //sh(RUN_EXEC);
}
