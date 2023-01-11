#include <stdio.h>
#include <stdlib.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/token.h"
#include "include/parser.h"
#include "include/as_frontend.h"
#include "include/stack.h"

#define OUTPUT_FILE "output.s"
#define COMPILE_OBJ "as output.s -o output.o"
#define MAKE_EXECUTABLE "ld output.o -o output -macosx_version_min 13.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem"
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

    printf("\nTokenizing bet source file '%s'...\n\n", src);
    Lexer* lexer = init_lexer(src_file);

    Parser* parser = init_parser(lexer);
    printf("\nParsing bet source file '%s'...\n\n", src);
    parser_parse_tokens(parser);

    print_ast(parser);

    Stack* stack_frame = init_stack();
    printf("Assembling bet source file '%s'...\n\n", src);
    char* output = assemble(parser->root, stack_frame);

    //print_stack_frame(stack_frame);

    printf("Compiling...\n\n");
    write_file(OUTPUT_FILE, output);
    system(COMPILE_OBJ);
    system(MAKE_EXECUTABLE);
    system(RUN_EXEC);
}
