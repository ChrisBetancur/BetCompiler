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

char* x86_assemble(ASTNode* node, SymbolTable* table, Proc* proc) {
    char* next_val = 0;
    switch (node->type) {
        case AST_GLOBAL:
            next_val = x86_global(node, table);
            break;

        case AST_INT:
            next_val = x86_int(node);
            break;

        case AST_BINARY_OP:
            next_val = x86_binary_op(node, table, proc);
            break;

        case AST_VAR:
            next_val = x86_var(node, table, proc);
            break;

        case AST_BUILT_IN:
            next_val = x86_built_in(node, table, proc);
            break;

        case AST_BLOCK:
            next_val = x86_func_block(node, table, proc);
            break;

        case AST_RETURN:
            next_val = x86_return(node, table, proc);
            break;

        case AST_FUNC:
            next_val = x86_func(node, table);
            break;

        case AST_CALL:
            next_val = x86_func_call(node, table, proc);
            break;

        default:
            x86_error_handler(UNEXPECTED_NODE, node);
            break;
    }

    return next_val;
}

char* x86_binary_op(ASTNode* node, SymbolTable* table, Proc* proc) {
    char* op;


    char* left = x86_assemble(node->children->arr[1], table, proc);
    char* right = x86_assemble(node->children->arr[2], table, proc);


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


char* x86_var_call(ASTNode* node, SymbolTable* table, Proc* proc) {
    const char* call_var_template = "\n    mov rdi, [%s %s 0x%x]\n"
                                      "    push rdi\n";

    char* op = NULL;

    if (strcmp(proc->name, "global") == 0) {
        op = "+";
    }
    else {
        op = "-";
    }

    if (!symbol_in_scope(table, proc, node->name)) {
        x86_error_handler(SYMBOL_NOT_IN_SCOPE, node);
    }

    Entry* entry = symbol_table_lookup(table, node->name);

    if (entry == NULL) {
        x86_error_handler(UNDEFINED_VAR, node);
    }

    char* output = calloc(strlen(call_var_template) + strlen(entry->base) + strlen(op) + sizeof(entry->mem_addr) + 1, sizeof(char));
    sprintf(output, call_var_template, entry->base, op, entry->mem_addr);

    return output;
}

char* x86_correct_base(char* proc) {
    if (strcmp(proc, "global") == 0) {
        return "var_stack";
    }
    return "rbp";
}

char* x86_int_var_assignment(ASTNode* node, SymbolTable* table, Proc* proc) {
    if (((ASTNode*) node->children->arr[1])->type == AST_EXPR) {
        return x86_eval_expr(((ASTNode*) node->children->arr[1])->children->arr[0], table, proc);
    }
    else {
        printf("Invalid assignment to var, should be int\n");
        exit(1);
    }
}

char* x86_mem_op(char* proc_name) {
    if (strcmp(proc_name, "global") == 0) {
        return "+";
    }
    else {
        return "-";
    }
}

char* x86_declared_var(ASTNode* node, SymbolTable* table, Proc* proc) {

    char* base = x86_correct_base(proc->name);

    ASTNode* child = node->children->arr[0];
    char* op = x86_mem_op(proc->name);

    Entry* entry = init_entry_mem(node->name, ENTRY_INT, base, proc->offset += 0x8);
    symbol_table_insert(table, proc, entry);

    const char* stack_push_template = "\n%s    pop rsi\n"
                                          "    mov QWORD [%s %s 0x%x], rsi\n\n";

    const char* stack_push_no_expr_template = "\n    mov rsi, 0\n"
                                                "    mov QWORD [%s %s 0x%x], rsi\n";

    const char* stack_push_call_template = "\n%s    mov rsi, rax\n"
                                               "    mov QWORD [%s %s 0x%x], rsi\n";

    char* output = NULL;

    if (((ASTNode*) node->children->arr[1])->type == AST_CALL) {
        char* func_call = x86_assemble((ASTNode*) node->children->arr[1], table, proc);
        output = calloc(strlen(func_call) + strlen(stack_push_call_template) + strlen(base) + strlen(op) + sizeof(entry->mem_addr) + 1, sizeof(char));
        sprintf(output, stack_push_call_template, func_call, base, op, entry->mem_addr);
        return output;
    }

    char* value = NULL;
    if (strcmp(child->name, "int") == 0) {
        value = x86_int_var_assignment(node, table, proc);
    }

    if (value != NULL) {
        output = calloc(strlen(stack_push_template) + strlen(value) + strlen(base) + strlen(op) + sizeof(entry->mem_addr) + 1, sizeof(char));
        sprintf(output, stack_push_template, value, base, op, entry->mem_addr);
        return output;
    }

    output = calloc(strlen(stack_push_no_expr_template) + strlen(base) + sizeof(entry->mem_addr) + 1, sizeof(char));

    sprintf(output, stack_push_no_expr_template, base, node->offset);

    free(entry);
    return output;

}

char* x86_var_reassign(ASTNode* node, SymbolTable* table, Proc* proc) {
    char* base = x86_correct_base(proc->name);

    ASTNode* child = node->children->arr[0];
    const char* stack_push_template = "%s\n    pop rsi\n"
                                          "    mov [%s + 0x%x], rsi\n\n";

    char* expr = x86_eval_expr(child->children->arr[0], table, proc);

    if (!symbol_in_scope(table, proc, node->name)) {
        x86_error_handler(SYMBOL_NOT_IN_SCOPE, node);
    }

    Entry* entry_stored = symbol_table_lookup(table, node->name);

    if (entry_stored == NULL) {
        x86_error_handler(UNDEFINED_VAR, node);
    }

    char* output = calloc(strlen(stack_push_template) + strlen(expr) + strlen(base) + sizeof(node->offset) + 1, sizeof(char));
    sprintf(output, stack_push_template, expr, base, entry_stored->mem_addr);

    return output;

}

char* x86_var(ASTNode* node, SymbolTable* table, Proc* proc) { // SHOULD PASS A SCOPE ARGUMENT IN ORDER TO PUSH A VALID ENTRY, FOR NOW ITS JUST GLOBAL
    if (node->children->num_items == 0) {
        return x86_var_call(node, table, proc);
    }

    ASTNode* child = node->children->arr[0];

    if (child->type == AST_DEC_TYPE) {
        return x86_declared_var(node, table, proc);
    }
    else {
        return x86_var_reassign(node, table, proc);
    }

    return NULL;
}

char* x86_eval_expr(ASTNode* node, SymbolTable* table, Proc* proc) {
    if (node->type != AST_BINARY_OP)
        return x86_int(node);

    char* output = x86_binary_op(node, table, proc);
    return output;
}

char* x86_print_element(ASTNode* element, SymbolTable* table, Proc* proc) {
    if (element->type == AST_BINARY_OP) {
        char* expr =  x86_eval_expr(element, table, proc);
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
        char* var_call = x86_var(element, table, proc);
        const char* print_var_template = "%s\n    pop rax\n"
                                             "    call print_int\n";

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

char* x86_built_in(ASTNode* node, SymbolTable* table, Proc* proc) {
    if (strcmp(node->name, "puts") == 0) {
        ASTNode* element = ((ASTNode*) node->children->arr[0])->children->arr[0];

        return x86_print_element(element, table, proc);
    }

    else if (strcmp(node->name, "print") == 0) {
        ASTNode* params = node->children->arr[0];

        char* output = NULL;
        for (int i = 0; i < params->children->num_items; i++) {
            char* print_output = x86_print_element(params->children->arr[i], table, proc);

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

// TODO: Identify duplicate literals to not repeat and seperate string and int literals; MIGHT HAVE TO CHANGE WHEN STRING VARS ARE INTRODUCED
char* x86_identify_literals(char* root_data, ASTNode* node, SymbolTable* table, Proc* proc) {
    if (node->type == AST_STRING && symbol_table_lookup(table, node->name) == NULL) {
        const char* literal_template = "%s    %s db '%s'\n"
                                       "    %s_len equ $ -%s\n\n";

        Entry* entry = init_entry_label(node->name, ENTRY_STRING, &table->num_labels);
        symbol_table_insert(table, proc, entry);

        char* new_root_data = calloc(strlen(literal_template) + strlen(root_data) + strlen(node->name) + (strlen(entry->label) * 3) + 1, sizeof(char));
        sprintf(new_root_data, literal_template, root_data, entry->label, node->name, entry->label, entry->label);
        return new_root_data;
    }

    char* literals = root_data;
    for (int i = 0; i < node->children->num_items; i++) {
        literals = x86_identify_literals(literals, node->children->arr[i], table, proc);

    }

    return literals;
}

char* x86_func_call(ASTNode* node, SymbolTable* table, Proc* proc) { // error handle if not in scope and if params match, how to pass func_def_params
    const char* func_call_template = "%s\n"
                                     "    call %s\n";

    Proc* func_proc = symbol_table_lookup_proc(table, node->name);
    char* prep_params = x86_prep_stack_frame(node->children->arr[0], func_proc->def->children->arr[1]);

    char* output = calloc(strlen(func_call_template) + strlen(prep_params) + strlen(proc->name) + 1, sizeof(char));
    sprintf(output, func_call_template, prep_params, func_proc->name);

    return output;
}

char* x86_prep_stack_frame(ASTNode* func_call_params, ASTNode* func_def_params) {
    Stack* stack_frame = init_stack();

    char* params_output = calloc(1, sizeof(char));

    if (func_call_params->children->num_items != func_def_params->children->num_items) {
        //handle func call not having same num of params
        exit(1);
    }

    for (int i = func_def_params->children->num_items - 1; i >= 0; i--) {
        const char* param_template;

        if (i == 0) {
            param_template = "%s    mov rdi, %s\n";

        }
        else if (i == 1) {
            param_template = "%s    mov rsi, %s\n";
        }
        else {
            param_template = "%s    push %s\n";
        }
        //puts(((ASTNode*) func_call_params->children->arr[i])->name);
        params_output = realloc(params_output, (strlen(param_template) + strlen(params_output) + strlen(((ASTNode*) func_call_params->children->arr[i])->name) + 1) * sizeof(char));
        sprintf(params_output, param_template, params_output, ((ASTNode*) func_call_params->children->arr[i])->name);
    }

    return params_output;
}


char* x86_identify_funcs(ASTNode* node, SymbolTable* table) {
    char* subroutines = calloc(1, sizeof(char));

    for (int i = 0; i < node->children->num_items; i++) {
        if (((ASTNode*) node->children->arr[i])->type == AST_FUNC) {
            char* curr_node = x86_func(node->children->arr[i], table);
            subroutines = realloc(subroutines, (strlen(subroutines) + strlen(curr_node) + 1) * sizeof(char));
            strcat(subroutines, curr_node);
        }
    }

    return subroutines;
}

char* x86_return(ASTNode* node, SymbolTable* table, Proc* proc) {
    ASTNode* ret_val = node->children->arr[0];
    Entry* return_entry = symbol_table_lookup(table, ret_val->name);

    const char* return_template = "    mov rax, QWORD [rbp - 0x%x]\n";

    char* output = calloc(strlen(return_template) + sizeof(return_entry->mem_addr) + 1, sizeof(char));
    sprintf(output, return_template, return_entry->mem_addr);
    return output;
}

char* x86_func_block(ASTNode* node, SymbolTable* table, Proc* proc) {
    char* block = calloc(1, sizeof(char));
    for (int i = 0; i < node->children->num_items; i++) {
        puts(astnode_to_string(node->children->arr[i]));
        char* curr_output = x86_assemble(node->children->arr[i], table, proc);
        block = realloc(block, (strlen(block) + strlen(curr_output) + 1) * sizeof(char));
        strcat(block, curr_output);
    }

    return block;
}

char* x86_func(ASTNode* node, SymbolTable* table) {
    const char* subroutine_template = "%s:\n"
                                      "    push rbp\n"
                                      "    mov rbp, rsp\n"
                                      "    sub rsp, 0x%x\n\n"
                                      "%s"
                                      "%s"
                                      "%s"
                                      "    ret\n";

    ASTNode* params = node->children->arr[1];

    char* params_to_local = calloc(1, sizeof(char));

    //unsigned int offset = 0x8;
    unsigned char access_offset = 0x8;

    Proc* proc = init_proc(node);
    symbol_table_insert_proc(table, proc);

    for (int i = 0; i < params->children->num_items; i++) {
        ASTNode* param_type = ((ASTNode*) params->children->arr[i])->children->arr[0];


        Entry* entry = init_entry_mem(((ASTNode*)params->children->arr[i])->name, ENTRY_INT, "rbp", proc->offset += 0x8);
        symbol_table_insert(table, proc, entry);

        if (i > 1) {
            access_offset += 0x8;
            const char* param_init_template = "%s    mov r8, QWORD [rbp + 0x%x]\n"
                                                "    mov QWORD [rbp - 0x%x], r8\n";

            params_to_local = realloc(params_to_local, (strlen(params_to_local) + strlen(param_init_template) + sizeof(access_offset) + sizeof(proc->offset) + 1) * sizeof(char));
            sprintf(params_to_local, param_init_template, params_to_local, access_offset, proc->offset);
        }

        else {
            const char* param_init_template;
            if (i == 1)
                param_init_template = "%s    mov QWORD [rbp - 0x%x], rsi\n";

            else
                param_init_template = "%s    mov QWORD [rbp - 0x%x], rdi\n";

            params_to_local = realloc(params_to_local, (strlen(param_init_template) + strlen(params_to_local) + sizeof(proc->offset) + 1) * sizeof(char));
            sprintf(params_to_local, param_init_template, params_to_local, proc->offset);
        }
    }

    char* body = x86_assemble(node->children->arr[2], table, proc);
    //puts(body);

    char* epilogue = "    mov rsp, rbp\n"
                     "    pop rbp\n";

    char* output = calloc(strlen(subroutine_template) + strlen(node->name) + sizeof(proc->offset) + strlen(params_to_local) + strlen(body) + strlen(epilogue) + 1, sizeof(char));

    sprintf(output, subroutine_template, node->name, proc->offset, params_to_local, body, epilogue);
    return output;
}

char* x86_global(ASTNode* node, SymbolTable* table) {
    Proc* proc = init_proc(node);
    symbol_table_insert_proc(table, proc);

    char* start_root = read_file(START_ROOT);

    char* end_root = read_file(END_ROOT);

    char* root_data = read_file(ROOT_DATA);
    root_data = x86_identify_literals(root_data, node, table, proc);

    char* root_bss = read_file(ROOT_BSS);

    char* root_const = read_file(ROOT_CONST);


    char* body_output = calloc(1, sizeof(char));
    size_t body_size = 0;


    char* subroutines = x86_identify_funcs(node, table);

    for (int i = 0; i < node->children->num_items; i++) {
        if (((ASTNode*) node->children->arr[i])->type != AST_FUNC) {
            char* curr_node = x86_assemble(node->children->arr[i], table, proc);

            body_size += strlen(curr_node) * sizeof(char);

            body_output = realloc(body_output, body_size + 1);
            strcat(body_output, curr_node);
        }
    }

    char* include_print = "\n\%include \"target/print_int.asm\"\n\n"; // FIX HERE, IT IS NOT READING THE INCLUDE
    char* buffer = calloc(strlen(start_root) + strlen(body_output) + strlen(end_root) + strlen(subroutines) + strlen(include_print)  + strlen(root_const) + strlen(root_data) + strlen(root_bss) + 1, sizeof(char)); //4 keeps track of the number of new lines in sprintf

    sprintf(buffer, "%s%s%s%s%s%s%s%s", start_root, body_output, end_root, subroutines, include_print, root_const, root_data, root_bss);

    return buffer;
}
