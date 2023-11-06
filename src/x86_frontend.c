#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "include/node.h"
#include "include/util.h"
#include "include/x86_frontend.h"
#include "include/io.h"
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

char* x86_print_element(node_t *element, proc_t *proc);

char* x86_built_in(node_t *node, proc_t *proc);

char* x86_op_evaluate(char*op);

char* x86_var_call(node_t *node, proc_t *proc);

char* x86_correct_base(char *proc);

char* x86_int_var_assignment(node_t *node, proc_t *proc);

char* x86_mem_op(char *proc_name);

char* x86_declared_var(node_t *node, proc_t *proc);

char* x86_var_reassign(node_t *node, proc_t *proc);

char* x86_var(node_t *node, proc_t *proc);

char* x86_int(node_t *node);

char* x86_binary_op(node_t *node, proc_t *proc);

char* x86_eval_expr(node_t *node, proc_t *proc);

char* x86_identify_literals(char *root_data, node_t *node, proc_t *proc);

char* x86_identify_funcs(node_t *node);

char* x86_return(node_t *node, proc_t *proc);

char* x86_func_block(node_t *node, proc_t *proc);

char* x86_func_call(node_t *node, proc_t *proc);

char* x86_prep_stack_frame(node_t *func_call_params, node_t *func_def_params);

char* x86_func_params(node_t *node, proc_t *proc);

char* x86_func(node_t *node);

char* x86_global(node_t *node);

/*
 * Function: x86_assemble
 *
 * Calls the correct function to convert current node in the AST into assembly
 *
 * node: Current node to assmeble
 * stack_frame:
 */

char* x86_assemble(node_t* node, proc_t* proc) {
    char *next_val = 0;
    switch (node->type) {
        case AST_GLOBAL:
            next_val = x86_global(node);
            break;

        case AST_INT:
            next_val = x86_int(node);
            break;

        case AST_BINARY_OP:
            next_val = x86_binary_op(node, proc);
            break;

        case AST_VAR:
            next_val = x86_var(node, proc);
            break;

        case AST_BUILT_IN:
            next_val = x86_built_in(node, proc);
            break;

        case AST_BLOCK:
            next_val = x86_func_block(node, proc);
            break;

        case AST_RETURN:
            next_val = x86_return(node, proc);
            break;

        case AST_FUNC:
            next_val = x86_func(node);
            break;

        case AST_CALL:
            next_val = x86_func_call(node, proc);
            break;

        default:
            frontend_error_handler(UNEXPECTED_NODE, node);
            break;
    }

    return next_val;
}

char* x86_binary_op(node_t *node, proc_t *proc) {
    char *op;


    char *left = x86_assemble(node->children->arr[1], proc);
    char *right = x86_assemble(node->children->arr[2], proc);


    if (strcmp(((node_t*) node->children->arr[0])->name, "+") == 0){ // does not check if adding vars together
        op = read_file(ADD);
    }

    if (strcmp(((node_t*) node->children->arr[0])->name, "-") == 0){ // does not check if adding vars together
        op = read_file(SUB);
    }

    if (strcmp(((node_t*) node->children->arr[0])->name, "*") == 0){ // does not check if adding vars together
        op = read_file(MUL);
    }

    if (strcmp(((node_t*) node->children->arr[0])->name, "/") == 0){ // does not check if adding vars together
        op = read_file(DIV);
    }


    char *output = calloc(strlen(left) + strlen(right) + strlen(op) + 1, sizeof(char));

    sprintf(output, "%s%s%s", left, right, op);

    return output;
}

char* x86_int(node_t *node) {
    const char *push_int_template = "    mov rax, %s\n"
                                    "    push rax\n";
    char *output = calloc(strlen(push_int_template) + strlen(node->name) + 1, sizeof(char));
    sprintf(output, push_int_template, node->name);

    return output;
}


char* x86_var_call(node_t *node, proc_t *proc) {
    const char *call_var_template = "\n    mov rdi, [%s %s 0x%x]\n"
                                      "    push rdi\n";

    char *op = NULL;

    if (strcmp(proc->name, "global") == 0) {
        op = "+";
    }
    else {
        op = "-";
    }

    if (!symbol_in_scope(table, proc, node->name)) {
        frontend_error_handler(SYMBOL_NOT_IN_SCOPE, node);
    }

    entry_t *entry = symbol_table_lookup(table, node->name);

    if (entry == NULL) {
        frontend_error_handler(UNDEFINED_VAR, node);
    }

    char *output = calloc(strlen(call_var_template) + strlen(entry->base) + strlen(op) + sizeof(entry->mem_addr) + 1, sizeof(char));
    sprintf(output, call_var_template, entry->base, op, entry->mem_addr);

    return output;
}

char* x86_correct_base(char *proc) {
    if (strcmp(proc, "global") == 0) {
        return "var_stack";
    }
    return "rbp";
}

char* x86_int_var_assignment(node_t *node, proc_t *proc) {
    if (((node_t*) node->children->arr[1])->type == AST_EXPR) {
        return x86_eval_expr(((node_t*) node->children->arr[1])->children->arr[0], proc);
    }
    else {
        frontend_error_handler(UNEXPECTED_ASSIGNMENT, node);
    }

    return NULL;
}

char* x86_mem_op(char *proc_name) {
    if (strcmp(proc_name, "global") == 0) {
        return "+";
    }
    else {
        return "-";
    }
}

char* x86_declared_var(node_t *node, proc_t *proc) {
    char *base = x86_correct_base(proc->name);

    node_t *child = node->children->arr[0];
    char *op = x86_mem_op(proc->name);

    entry_t *entry = init_entry_mem(node->name, ENTRY_INT, base, proc->offset += type_size(child->name));

    char *value = NULL;

    symbol_table_insert(table, proc, entry);

    const char *stack_push_template = "\n%s    pop rsi\n"
                                          "    mov [%s %s 0x%x], rsi\n\n";

    const char *stack_push_no_expr_template = "\n    mov rsi, 0\n"
                                                "    mov [%s %s 0x%x], rsi\n";

    const char *stack_push_call_template = "\n%s    mov rsi, rax\n"
                                               "    mov [%s %s 0x%x], rsi\n";

    char *output = NULL;

    if (strcmp(child->name, "i32") == 0) {
        printf("[DEBUG] Unable to reserve less than 64 bits for an integer\n");
        exit(1);
    }

    if (((node_t*) node->children->arr[1])->type == AST_CALL) {
        proc_t *func_proc = symbol_table_lookup_proc(table, ((node_t*)node->children->arr[1])->name);
        if (strcmp(((node_t*) func_proc->def->children->arr[0])->name, "void") == 0) {
            frontend_error_handler(UNEXPECTED_ASSIGNMENT, node);
        }
        char *func_call = x86_assemble((node_t*) node->children->arr[1], proc);
        output = calloc(strlen(func_call) + strlen(stack_push_call_template) + strlen(base) + strlen(op) + sizeof(entry->mem_addr) + 1, sizeof(char));
        sprintf(output, stack_push_call_template, func_call, base, op, entry->mem_addr);
        return output;
    }

    if (strcmp(child->name, "i64") == 0 || strcmp(child->name, "i32") == 0) {
        value = x86_int_var_assignment(node, proc);
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

char* x86_var_reassign(node_t *node, proc_t *proc) {
    char *base = x86_correct_base(proc->name);

    node_t *child = node->children->arr[0];
    const char *stack_push_template = "%s\n    pop rsi\n"
                                          "    mov [%s + 0x%x], rsi\n\n";

    char *expr = x86_eval_expr(child->children->arr[0], proc);

    if (!symbol_in_scope(table, proc, node->name)) {
        frontend_error_handler(SYMBOL_NOT_IN_SCOPE, node);
    }

    entry_t *entry_stored = symbol_table_lookup(table, node->name);

    if (entry_stored == NULL) {
        frontend_error_handler(UNDEFINED_VAR, node);
    }

    char *output = calloc(strlen(stack_push_template) + strlen(expr) + strlen(base) + sizeof(node->offset) + 1, sizeof(char));
    sprintf(output, stack_push_template, expr, base, entry_stored->mem_addr);

    return output;

}

char* x86_var(node_t *node, proc_t *proc) { // SHOULD PASS A SCOPE ARGUMENT IN ORDER TO PUSH A VALID ENTRY, FOR NOW ITS JUST GLOBAL
    if (node->children->num_items == 0) {
        return x86_var_call(node, proc);
    }

    node_t *child = node->children->arr[0];

    if (child->type == AST_DEC_TYPE) {
        return x86_declared_var(node, proc);
    }
    else {
        return x86_var_reassign(node, proc);
    }

    return NULL;
}

char* x86_eval_expr(node_t *node, proc_t *proc) {
    if (node->type != AST_BINARY_OP)
        return x86_int(node);

    char *output = x86_binary_op(node, proc);
    return output;
}

char* x86_print_element(node_t *element, proc_t *proc) {
    if (element->type == AST_BINARY_OP) {
        char *expr =  x86_eval_expr(element, proc);
        char *output = calloc(strlen(expr) + strlen("    pop rax\n    call print_int\n") + 1, sizeof(char));
        sprintf(output, "%s    pop rax\n    call print_int\n", expr);

        return output;
    }

    if (element->type == AST_INT) {
        const char *print_int_template = "    mov rax, %s\n"
                                         "    call print_int\n";
        char *output = calloc(strlen(print_int_template) + strlen(element->name) + 1, sizeof(char));
        sprintf(output, print_int_template, element->name);
        return output;
    }

    if (element->type == AST_VAR) {
        char* var_call = x86_var(element, proc);
        const char* print_var_template = "%s\n    pop rax\n"
                                             "    call print_int\n";

        char *output = calloc(strlen(var_call) + strlen(print_var_template) + 1, sizeof(char));
        sprintf(output, print_var_template, var_call);

        return output;
    }

    if (element->type == AST_STRING) {
        const char *print_literal_str_template = "    mov rax, SYS_WRITE\n"
                                                 "    mov rdi, STDOUT\n"
                                                 "    mov rsi, %s\n"
                                                 "    mov rdx, %s_len\n"
                                                 "    syscall\n";

        entry_t *entry = symbol_table_lookup(table, element->name);

        char *output = calloc(strlen(print_literal_str_template) + strlen(entry->label) * 2, sizeof(char));
        sprintf(output, print_literal_str_template, entry->label, entry->label, sizeof(char));

        return output;
    }

    return NULL;
}

char* x86_built_in(node_t *node, proc_t *proc) {
    if (strcmp(node->name, "puts") == 0) {
        node_t *element = ((node_t*) node->children->arr[0])->children->arr[0];

        return x86_print_element(element, proc);
    }

    else if (strcmp(node->name, "print") == 0) {
        node_t *params = node->children->arr[0];

        char *output = NULL;
        for (int i = 0; i < params->children->num_items; i++) {
            char *print_output = x86_print_element(params->children->arr[i], proc);

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
char* x86_identify_literals(char *root_data, node_t *node, proc_t *proc) {
    if (node->type == AST_STRING && symbol_table_lookup(table, node->name) == NULL) {
        const char *literal_template = "%s    %s db '%s'\n"
                                       "    %s_len equ $ -%s\n\n";

        entry_t *entry = init_entry_label(node->name, ENTRY_STRING, &table->num_labels);
        symbol_table_insert(table, proc, entry);

        char *new_root_data = calloc(strlen(literal_template) + strlen(root_data) + strlen(node->name) + (strlen(entry->label) * 3) + 1, sizeof(char));
        sprintf(new_root_data, literal_template, root_data, entry->label, node->name, entry->label, entry->label);
        return new_root_data;
    }

    char *literals = root_data;
    for (int i = 0; i < node->children->num_items; i++) {
        literals = x86_identify_literals(literals, node->children->arr[i], proc);

    }

    return literals;
}

char* x86_func_call(node_t *node, proc_t *proc) { // error handle if not in scope and if params match, how to pass func_def_params
    const char *func_call_template = "%s\n"
                                     "    call %s\n";

    proc_t *func_proc = symbol_table_lookup_proc(table, node->name);
    char* prep_params = x86_prep_stack_frame(node->children->arr[0], func_proc->def->children->arr[1]);

    char* output = calloc(strlen(func_call_template) + strlen(prep_params) + strlen(proc->name) + 1, sizeof(char));
    sprintf(output, func_call_template, prep_params, func_proc->name);

    return output;
}

char* x86_prep_stack_frame(node_t *func_call_params, node_t *func_def_params) {
    //Stack* stack_frame = init_stack();

    char *params_output = calloc(1, sizeof(char));

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
        params_output = realloc(params_output, (strlen(param_template) + strlen(params_output) + strlen(((node_t*) func_call_params->children->arr[i])->name) + 1) * sizeof(char));
        sprintf(params_output, param_template, params_output, ((node_t*) func_call_params->children->arr[i])->name);
    }

    return params_output;
}


char* x86_identify_funcs(node_t *node) {
    char *subroutines = calloc(1, sizeof(char));

    for (int i = 0; i < node->children->num_items; i++) {
        if (((node_t*) node->children->arr[i])->type == AST_FUNC) {
            char *curr_node = x86_func(node->children->arr[i]);
            subroutines = realloc(subroutines, (strlen(subroutines) + strlen(curr_node) + 1) * sizeof(char));
            strcat(subroutines, curr_node);
        }
    }

    return subroutines;
}

char* x86_return(node_t *return_node, proc_t *proc) { // HANDLE SCOPE HERE??

    node_t *func_node = proc->def;
    node_t *type = func_node->children->arr[0];

    if (strcmp(type->name, "void") == 0) {

        if (return_node->children->num_items == 0) return "";

        puts("invalid return value for void");
        exit(1);
    }

    if (strcmp(type->name, "i64") == 0 || strcmp(type->name, "i32") == 0) {
        if (return_node->children->num_items == 0) {
            puts("invalid return value for i64");
            exit(1);
        }

        node_t *ret_val = return_node->children->arr[0];

        if (is_expression(ret_val->name)) {
            const char *return_template = "    mov rax, %s\n";

            char *output = calloc(strlen(return_template) + strlen(ret_val->name) + 1, sizeof(char));
            sprintf(output, return_template, ret_val->name);

            return output;
        }

        entry_t *return_entry = symbol_table_lookup(table, ret_val->name);

        const char *return_template = "    mov rax, [rbp - 0x%x]\n";

        char *output = calloc(strlen(return_template) + sizeof(return_entry->mem_addr) + 1, sizeof(char));
        sprintf(output, return_template, return_entry->mem_addr);
        return output;
    }

    printf("invalid return type %s\n", astnode_to_string(return_node->children->arr[0]));
    exit(1);

}

char* x86_func_block(node_t *node, proc_t *proc) {
    bool requires_return = false;

    if (strcmp(((node_t*) proc->def->children->arr[0])->name, "void") != 0) {
        requires_return = true;
    }

    bool has_return = false;

    char *block = calloc(1, sizeof(char));
    for (int i = 0; i < node->children->num_items; i++) {
        if (((node_t*) node->children->arr[i])->type == AST_RETURN)
            has_return = true;
        char *curr_output = x86_assemble(node->children->arr[i], proc);
        block = realloc(block, (strlen(block) + strlen(curr_output) + 1) * sizeof(char));
        strcat(block, curr_output);
    }

    if (!has_return && requires_return) {
        printf("Function needs return value\n");
        exit(1);
    }
    return block;
}

char* x86_func_params(node_t *node, proc_t *proc) {
    char *params_to_local = calloc(1, sizeof(char));

    //unsigned int offset = 0x8;
    unsigned char access_offset = 0x8;

    for (int i = 0; i < node->children->num_items; i++) {
        node_t *param_type = ((node_t*) node->children->arr[i])->children->arr[0];

        entry_t *entry = init_entry_mem(((node_t*)node->children->arr[i])->name, ENTRY_INT, "rbp", proc->offset += (type_size(param_type->name)));
        symbol_table_insert(table, proc, entry);

        if (i > 1) { // FIX THIS 0X8 OFFSET WHEN THERE IS MORE THAN 2 PARAMS
            access_offset += 0x8;
            const char* param_init_template = "%s    mov r8, [rbp + 0x%x]\n"
                                                "    mov [rbp - 0x%x], r8\n";

            params_to_local = realloc(params_to_local, (strlen(params_to_local) + strlen(param_init_template) + sizeof(access_offset) + sizeof(proc->offset) + 1) * sizeof(char));
            sprintf(params_to_local, param_init_template, params_to_local, access_offset, proc->offset);
        }

        else {
            const char *param_init_template;
            if (i == 1)
                param_init_template = "%s    mov [rbp - 0x%x], rsi\n";

            else
                param_init_template = "%s    mov [rbp - 0x%x], rdi\n";


            params_to_local = realloc(params_to_local, (strlen(param_init_template) + strlen(params_to_local) + sizeof(proc->offset) + 1) * sizeof(char));
            sprintf(params_to_local, param_init_template, params_to_local, proc->offset);
        }
    }

    return params_to_local;
}

char* x86_func(node_t *node) {
    const char* subroutine_template = "\n%s:\n"
                                      "    push rbp\n"
                                      "    mov rbp, rsp\n"
                                      "    sub rsp, 0x%x\n\n"
                                      "%s"
                                      "%s"
                                      "%s"
                                      "    ret\n";

    node_t *params = node->children->arr[1];

    proc_t *proc = init_proc(node);
    symbol_table_insert_proc(table, proc);
    proc->offset = 0x10;

    char *params_to_local = x86_func_params(params, proc);

    char *body = x86_assemble(node->children->arr[2], proc);
    //puts(body);

    char *epilogue = "    mov rsp, rbp\n"
                     "    pop rbp\n";

    //char stack_frame_alloc = align_stack_frame(proc->offset); // SHOULD I LEAVE PARAMS AS 8 BYTE VALUES??? SOMETHING IS NOT WORKING NOT DETECTING PARAMS WHEN IN I32 BIT MOD, SHOULD FIX ABOVE PARAM ISSUES BEFORE THIS MAYBE?

    char *output = calloc(strlen(subroutine_template) + strlen(node->name) + sizeof(proc->offset) + strlen(params_to_local) + strlen(body) + strlen(epilogue) + 1, sizeof(char));

    sprintf(output, subroutine_template, node->name, proc->offset, params_to_local, body, epilogue);
    return output;
}

char* x86_global(node_t *node) {
    proc_t *proc = init_proc(node);
    symbol_table_insert_proc(table, proc);

    char *start_root = read_file(START_ROOT);

    char *end_root = read_file(END_ROOT);

    char *root_data = read_file(ROOT_DATA);
    root_data = x86_identify_literals(root_data, node, proc);

    char *root_bss = read_file(ROOT_BSS);

    char *root_const = read_file(ROOT_CONST);


    char *body_output = calloc(1, sizeof(char));
    size_t body_size = 0;


    char *subroutines = x86_identify_funcs(node);

    for (int i = 0; i < node->children->num_items; i++) {
        if (((node_t*) node->children->arr[i])->type != AST_FUNC) {
            char *curr_node = x86_assemble(node->children->arr[i], proc);

            body_size += strlen(curr_node) * sizeof(char);

            body_output = realloc(body_output, body_size + 1);
            strcat(body_output, curr_node);
        }
    }

    char *include_print = "\n\%include \"target/print_int.asm\"\n\n";
    char *buffer = calloc(strlen(start_root) + strlen(body_output) + strlen(end_root) + strlen(subroutines) + strlen(include_print)  + strlen(root_const) + strlen(root_data) + strlen(root_bss) + 1, sizeof(char)); //4 keeps track of the number of new lines in sprintf

    sprintf(buffer, "%s%s%s%s%s%s%s%s", start_root, body_output, end_root, subroutines, include_print, root_const, root_data, root_bss);

    return buffer;
}
