#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "include/ast_node.h"
#include "include/util.h"
#include "include/x86_frontend.h"
#include "include/io.h"
#include "include/stack.h"
#include "include/symbol_table.h"

#define PATH "/home/c_bet/Projects/BetCompiler/src/x86_asm/"
#define START_ROOT PATH "start_root.asm"
#define END_ROOT PATH "end_root.asm"
#define ROOT_DATA PATH"root_data.asm"
#define ROOT_BSS PATH "root_bss.asm"
#define ROOT_CONST PATH "root_const.asm"
#define PRINT_INT PATH "Subroutines/print_int.asm"
#define ADD PATH "add.asm"
#define SUB PATH "sub.asm"
#define MUL PATH "mul.asm"
#define DIV PATH "div.asm"

/*
 * Function: x86_assemble
 *
 * Calls the correct function to convert current node in the AST into assembly
 *
 * node: Current node to assmeble
 * stack_frame:
 */

char* x86_assemble(ASTNode* node, SymbolTable* table) {

    char* next_val = 0;

    switch (node->type) {
        case AST_GLOBAL:
            next_val = x86_global(node, table);
            break;

        case AST_INT:
            next_val = x86_int(node);
            break;

        case AST_BINARY_OP:
            next_val = x86_binary_op(node, table);
            break;

        case AST_VAR:
            next_val = x86_var(node, table);
            break;

        case AST_BUILT_IN:
            next_val = x86_built_in(node, table);
            break;

        default:
            x86_error_handler(UNEXPECTED_NODE, node);
            break;
    }

    return next_val;
}

char* x86_binary_op(ASTNode* node, SymbolTable* table) {
    char* op;


    char* left = x86_assemble(node->children->arr[1], table);
    char* right = x86_assemble(node->children->arr[2], table);


    if (strcmp(((ASTNode*) node->children->arr[0])->name, "+") == 0){ // does not check if adding vars together
        op = read_file(ADD);
    }

    if (strcmp(((ASTNode*) node->children->arr[0])->name, "-") == 0){ // does not check if adding vars together
        op = read_file(SUB);
    }

    if (strcmp(((ASTNode*) node->children->arr[0])->name, "*") == 0){ // does not check if adding vars together
        op = read_file(MUL);
    }

    if (strcmp(((ASTNode*) node->children->arr[0])->name, "/") == 0){ // does not check if adding vars together
        op = read_file(DIV);
    }


    char* output = calloc(strlen(left) + strlen(right) + strlen(op) + 1, sizeof(char));

    sprintf(output, "%s%s%s", left, right, op);

    return output;
}

char* x86_int(ASTNode* node) {
    const char* push_int_template = "    mov rax, %s\n"
                                    "    push rax\n";
    char* output = calloc(strlen(push_int_template) + strlen(node->name) + 1, sizeof(char));
    sprintf(output, push_int_template, node->name);

    return output;
}


char* x86_var_call(ASTNode* node, SymbolTable* table) {
    const char* call_var_template = "\n    mov rdi, [var_stack + %d]\n"
                                      "    push rdi\n";

    Entry* entry = symbol_table_lookup(table, node->name);

    puts(astnode_to_string(node));
    if (entry == NULL) {
        x86_error_handler(UNDEFINED_VAR, node);
    }

    char* output = calloc(strlen(call_var_template) + 1 + 1, sizeof(char));
    sprintf(output, call_var_template, entry->mem_addr);

    return output;
}

char* x86_var(ASTNode* node, SymbolTable* table) { // SHOULD PASS A SCOPE ARGUMENT IN ORDER TO PUSH A VALID ENTRY, FOR NOW ITS JUST GLOBAL
    if (node->children->num_items == 0) {
        return x86_var_call(node, table);
    }

    ASTNode* child = node->children->arr[0];

    if (child->type == AST_DEC_TYPE) {
        if (strcmp(child->name, "int") == 0) {
            Entry* entry = init_entry_mem(node->name, ENTRY_INT, table->top_offset += 0x10, PROC_GLOBAL);
            symbol_table_insert(table, entry);

            const char* stack_push_template = "\n%s    pop rsi\n"
                                                  "    mov [var_stack + %d], rsi\n\n";

            const char* stack_push_no_expr_template = "\n    mov rsi, 0\n"
                                                        "    mov [var_stack + %d], rsi\n";

            char* expr = NULL;
            if (((ASTNode*) node->children->arr[1])->type == AST_EXPR) {
                expr = x86_eval_expr(((ASTNode*) node->children->arr[1])->children->arr[0], table);
            }

            char* output;
            if (expr != NULL) {
                output = calloc(strlen(stack_push_template) + strlen(expr) + sizeof(node->offset) * 4 + 1, sizeof(char));
                sprintf(output, stack_push_template, expr, entry->mem_addr);
                return output;
            }

            output = calloc(strlen(stack_push_no_expr_template) + sizeof(node->offset) * 4 + 1, sizeof(char));

            sprintf(output, stack_push_no_expr_template, node->offset);

            return output;
        }
    }
    else {
        const char* stack_push_template = "%s\n    pop rsi\n"
                                            "    mov [var_stack + %d], rsi\n\n";

        char* expr = x86_eval_expr(child->children->arr[0], table);

        Entry* entry_stored = symbol_table_lookup(table, node->name);

        if (entry_stored == NULL) {
            x86_error_handler(UNDEFINED_VAR, node);
        }

        char* output = calloc(strlen(stack_push_template) + strlen(expr) + 1 + 1, sizeof(char));
        sprintf(output, stack_push_template, expr, entry_stored->mem_addr);

        return output;
    }

    return NULL;
}

char* x86_eval_expr(ASTNode* node, SymbolTable* table) {
    if (node->type != AST_BINARY_OP)
        return x86_int(node);
    char* output = x86_binary_op(node, table);
    return output;
}

char* x86_print_element(ASTNode* element, SymbolTable* table) {
    if (element->type == AST_BINARY_OP) {
        char* expr =  x86_eval_expr(element, table);
        char* output = calloc(strlen(expr) + strlen("    pop rax\n    call print_int\n") + 1, sizeof(char));
        sprintf(output, "%s    pop rax\n    call print_int\n", expr);

        return output;
    }

    if (element->type == AST_INT) {
        const char* print_int_template = "    mov rax, %s\n"
                                         "    call print_int\n";
        char* output = calloc(strlen(print_int_template) + strlen(element->name) + 1, sizeof(char));
        sprintf(output, print_int_template, element->name);
        return output;
    }

    if (element->type == AST_VAR) {
        char* var_call = x86_var(element, table);
        const char* print_var_template = "%s\n    pop rax\n"
                                            "     call print_int\n";

        char* output = calloc(strlen(var_call) + strlen(print_var_template) + 1, sizeof(char));
        sprintf(output, print_var_template, var_call);

        return output;
    }

    if (element->type == AST_STRING) {
        const char* print_literal_str_template = "    mov rax, SYS_WRITE\n"
                                                 "    mov rdi, STDOUT\n"
                                                 "    mov rsi, %s\n"
                                                 "    mov rdx, %s_len\n"
                                                 "    syscall\n";

        Entry* entry = symbol_table_lookup(table, element->name);

        char* output = calloc(strlen(print_literal_str_template) + strlen(entry->label) * 2, sizeof(char));
        sprintf(output, print_literal_str_template, entry->label, entry->label, sizeof(char));

        return output;
    }

    return NULL;
}

char* x86_built_in(ASTNode* node, SymbolTable* table) {
    if (strcmp(node->name, "puts") == 0) {
        ASTNode* element = ((ASTNode*) node->children->arr[0])->children->arr[0];

        return x86_print_element(element, table);
    }

    else if (strcmp(node->name, "print") == 0) {
        ASTNode* params = node->children->arr[0];

        char* output = NULL;
        for (int i = 0; i < params->children->num_items; i++) {
            char* print_output = x86_print_element(params->children->arr[i], table);

            if (output == NULL) {
                output = print_output;
            }

            else {
                output = realloc(output, (strlen(output) + strlen(print_output) + 2) * sizeof(char));
                strcat(output, print_output);
            }
        }

        return output;
    }

    return NULL;
}

// TODO: Identify duplicate literals to not repeat and seperate string and int literals
char* x86_identify_literals(char* root_data, ASTNode* node, SymbolTable* table) {
    if (node->type == AST_STRING && symbol_table_lookup(table, node->name) == NULL) {
        const char* literal_template = "%s    %s db '%s', 10\n"
                                       "    %s_len equ $ -%s\n\n";

        Entry* entry = init_entry_label(node->name, ENTRY_STRING, &table->num_labels, PROC_GLOBAL);
        symbol_table_insert(table, entry);

        char* new_root_data = calloc(strlen(literal_template) + strlen(root_data) + strlen(node->name) + (strlen(entry->label) * 3) + 1, sizeof(char));
        sprintf(new_root_data, literal_template, root_data, entry->label, node->name, entry->label, entry->label);
        return new_root_data;
    }

    char* literals = root_data;
    for (int i = 0; i < node->children->num_items; i++) {
        literals = x86_identify_literals(literals, node->children->arr[i], table);

    }

    return literals;
}


char* x86_global(ASTNode* node, SymbolTable* table) {
    char* start_root = read_file(START_ROOT);

    char* end_root = read_file(END_ROOT);

    char* root_data = read_file(ROOT_DATA);
    root_data = x86_identify_literals(root_data, node, table);

    char* root_bss = read_file(ROOT_BSS);

    char* root_const = read_file(ROOT_CONST);

    char* print_int = read_file(PRINT_INT);


    char* body_output = NULL;
    size_t body_size = 0;

    for (int i = 0; i < node->children->num_items; i++) {
        char* curr_node = x86_assemble(node->children->arr[i], table);

        body_size += strlen(curr_node) * sizeof(char);

        if (body_output == NULL) {
            body_output = calloc(strlen(curr_node) + 1, sizeof(char));
            strcpy(body_output, curr_node);
        }
        else {
            body_output = realloc(body_output, body_size + 1);
            strcat(body_output, curr_node);
        }
    }

    char* buffer = calloc(strlen(start_root) + strlen(body_output) + strlen(end_root) + strlen(print_int) + strlen(root_const) + strlen(root_data) + strlen(root_bss) + 1, sizeof(char)); //4 keeps track of the number of new lines in sprintf

    sprintf(buffer, "%s%s%s%s%s%s%s", start_root, body_output, end_root, print_int, root_const, root_data, root_bss);

    return buffer;
}
