#include <stdio.h>
#include <stdlib.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/token.h"
#include "include/parser.h"
#include "include/as_frontend.h"
#include "include/stack.h"
#include "include/x86_frontend.h"

#if __APPLE__
    #define OUTPUT_FILE "output.s"
    #define COMPILE_OBJ "as output.s -o output.o"
    #define MAKE_EXECUTABLE "ld output.o -o output -macosx_version_min 13.0 -L /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib -lSystem"
    #define RUN_EXEC "./output"
#elif defined(__x86_64__)
    #define OUTPUT_FILE "output.asm"
    #define COMPILE_OBJ "nasm -f elf64 -o output.o output.asm"
    #define MAKE_EXECUTABLE "ld -o output output.o"
    #define RUN_EXEC "./output"

#endif

#define AST_OUTPUT "abstract_syntax_tree.txt"
#define TOKENS_OUTPUT "tokens.txt"
/*
 * Function: bet_compile
 *
 * Takes the file contents and compiles
 *  1. Translates code to tokens
 *  2. Translates tokens to Abstract Syntax Tree
 */

void bet_compile(char* src) {
    char* src_file = read_file(src);

    //system("clear");
    printf("Tokenizing bet source file '%s'...\n", src);
    Lexer* lexer = init_lexer(src_file);

    Parser* parser = init_parser(lexer);
    printf("Parsing bet source file '%s'...\n", src);
    parser_parse_tokens(parser);

    //print_ast(parser);

    free(lexer);
    Stack* stack_frame = init_stack();
    printf("Assembling bet source file '%s'...\n", src);
    char* output = x86_assemble(parser->root, stack_frame);

    //print_stack_frame(stack_frame);

    printf("Compiling...\n\n");
    write_file(OUTPUT_FILE, output);
    system(COMPILE_OBJ);
    system(MAKE_EXECUTABLE);
    system(RUN_EXEC);
}


void bet_ide_compile(char* src) {
    char* src_file = read_file(src);

    Lexer* lexer = init_lexer(src_file);

    Parser* parser = init_parser(lexer);
    parser_parse_tokens(parser);

    FILE* ast_output_file = fopen(AST_OUTPUT, "w");

    ast_to_file(parser, ast_output_file);

    fclose(ast_output_file);

    free(lexer);

    FILE* token_output_file = fopen(TOKENS_OUTPUT, "w");
    tokens_to_file(parser, token_output_file);
    fclose(token_output_file);

    Stack* stack_frame = init_stack();
    char* output = x86_assemble(parser->root, stack_frame);

    //print_stack_frame(stack_frame);

    write_file(OUTPUT_FILE, output);
    system(COMPILE_OBJ);
    system(MAKE_EXECUTABLE);
    system(RUN_EXEC);

}
