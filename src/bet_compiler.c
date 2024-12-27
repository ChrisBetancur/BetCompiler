#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

#include "include/bet_compiler.h"
#include "include/lexer.h"
#include "include/io.h"
#include "include/token.h"
#include "include/parser.h"
#include "include/stack.h"
#include "include/symbol_table.h"
#include "include/x86_frontend.h"

#define TARGET_PATH "target/"
#define ASM_MAIN TARGET_PATH "main.asm"
#define ASM_PRINT_INT TARGET_PATH "print_int.asm"
#define NASM_OBJ "nasm -f elf64 -o "
#define MAIN_OBJ NASM_OBJ "target/main.o " ASM_MAIN
#define PRINT_INT_OBJ NASM_OBJ "target/print_int.o target/print_int.asm"
#define MAKE_EXECUTABLE "ld -o bin/output target/main.o target/print_int.o"
#define RUN_EXEC "bin/output"

#define AST_OUTPUT "abstract_syntax_tree.txt"
#define TOKENS_OUTPUT "tokens.txt"
#define TABLE_SIZE 15
/*
 * Function: bet_compile
 *
 * Takes the file contents and compiles
 *  1. Translates code to tokens
 *  2. Translates tokens to Abstract Syntax Tree
 */

/*void bet_compile(char* src) {
    char* src_file = read_file(src);

    //system("clear");
    printf("Tokenizing bet source file '%s'...\n", src);
    Lexer* lexer = init_lexer(src_file);

    Parser* parser = init_parser(lexer);
    printf("Parsing bet source file '%s'...\n", src);
    parser_parse_tokens(parser);

    //print_ast(parser);

    free(lexer);
    //Stack* stack_frame = init_stack();
    SymbolTable* table = init_symbol_table(15);
    printf("Assembling bet source file '%s'...\n", src);
    char* output = x86_assemble(parser->root, table, -1);

    puts(symbol_table_to_string(table));
    //print_stack_frame(stack_frame);

    printf("Compiling...\n\n");
    write_file(OUTPUT_FILE, output);
    system(COMPILE_OBJ);
    system(MAKE_EXECUTABLE);
    system(RUN_EXEC);
    printf("\n");
}*/


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

    /*SymbolTable* table = init_symbol_table(3);
    char* output = x86_assemble(parser->root, table, NULL);

    //puts(symbol_table_to_string(table));

    char* target_path = get_target_path();

    write_file(ASM_MAIN, output);

    char* print_int = get_print_int();
    write_file(ASM_PRINT_INT, print_int);

    system(PRINT_INT_OBJ);
    system(MAIN_OBJ);

    system(MAKE_EXECUTABLE);
    system(RUN_EXEC);

    puts("");*/
}

char* get_target_path() {
    char dir_path[PATH_MAX];

    if (getcwd(dir_path, sizeof(dir_path)) == NULL) {
        printf("[DEBUG] UNABLE TO GET WORKING DIR\n");
        exit(1);
    }

    strcat(dir_path, "/");
    strcat(dir_path, TARGET_PATH);

    char* target_path = &dir_path[0];
    return target_path;
}

char* get_print_int() {
    char dir_path[PATH_MAX];

    getcwd(dir_path, sizeof(dir_path));
    strcat(dir_path, "/src/x86_asm/Subroutines/print_int.asm");
    char* print_int = read_file(dir_path);

    return print_int;
}

