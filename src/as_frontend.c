#include "include/as_frontend.h"
#include "include/built_in.h"
#include "include/subroutines.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char* assemble(ASTNode* node, Stack* stack_frame) {
    char* output = calloc(1, sizeof(char));
    char* next_val = 0;


    switch (node->type) {
        case AST_GLOBAL:
            next_val = as_global(node, stack_frame);
            break;
        case AST_INT:
            next_val = as_int(node, stack_frame);
            break;

        case AST_VAR:
            next_val = as_var(node, stack_frame);
            break;

        case AST_BUILT_IN:
            next_val = as_built_in(node, stack_frame);
            break;

        default:
            printf("Assembler Front End: '%s' unknown Abstract Syntax Tree node type", astnode_type_to_string(node->type));
            exit(1);
    }

    output = realloc(output, (strlen(next_val) + 1) * sizeof(char));
    strcat(output, next_val);

    return output;
}


char* as_op_eval(char* op, char* reg1, char* reg2) { // only add and sub
    if (strcmp(op, "+") == 0) {
        const char* sum_template = "\tadd x3, %s, %s\n\n";
        char* sum_output = calloc(strlen(sum_template) + strlen(reg1) + strlen(reg2) + 1, sizeof(char));
        sprintf(sum_output, sum_template, reg1, reg2);
        return sum_output;
    }

    if (strcmp(op, "-") == 0) { // sub is not working
        const char* sub_template = "\tsub x3, %s, %s\n\n";
        char* sub_output = calloc(strlen(sub_template) + strlen(reg1) + strlen(reg2) + 1, sizeof(char));
        sprintf(sub_output, sub_template, reg1, reg2);
        return sub_output;
    }

    if (strcmp(op, "/") == 0) {
        const char* divide_template = "\tmov x3, #0\n%s\n";
        //char* divide_output = calloc(strlen(divide_template) + 1, sizeof(char));
        //strcpy(divide_output, divide_template);
        char* divide_sub = divide("x3", reg1, reg2);
        char* divide_output = calloc(strlen(divide_template) + strlen(divide_sub) + 1, sizeof(char));
        sprintf(divide_output, divide_template, divide_sub);
        return divide_output;
    }

    return NULL;
}


char* as_eval_expr(ASTNode* node, char* reg, Stack* stack_frame) { // evalutes ints not including calling vars in expressions yet
    char* eval_output = NULL;

    bool is_right_binop = false;
    bool is_left_binop = false;

    char* right_output = NULL;
    char* left_output = NULL;

    switch (node->type) { // check if AST_TERM is unused
        case AST_INT:
            if (reg == NULL) {
                const char* int_template = "\tmov x3, #%s\n";
                eval_output = calloc(strlen(int_template) + strlen(node->name), sizeof(char));
                sprintf(eval_output, int_template, node->name);
            }
            else {//??? if i takr out if statement defining const string does not work
                const char* int_template = "\tmov %s, #%s\n";

                eval_output = calloc(strlen(int_template) + strlen(reg) + strlen(node->name), sizeof(char));
                sprintf(eval_output, int_template, reg, node->name);
            }
            break;

        case AST_BINARY_OP:
            if (((ASTNode*) node->children->arr[1])->type == AST_BINARY_OP)
                is_right_binop = true;

            right_output = as_eval_expr(node->children->arr[1], "x0", stack_frame);

            if (((ASTNode*) node->children->arr[2])->type== AST_BINARY_OP)
                is_left_binop = true;
            left_output = as_eval_expr(node->children->arr[2], "x1", stack_frame);


            char* reg1;
            char* reg2;

            if (is_left_binop) {
                const char* reg1_template = "x3";
                const char* reg2_template = "x0";
                reg1 = calloc(strlen(reg1_template), sizeof(char));
                strcpy(reg1, reg1_template);
                reg2 = calloc(strlen(reg2_template), sizeof(char));
                strcpy(reg2, reg2_template);
            }
            else if (is_right_binop) {
                const char* reg1_template = "x3";
                const char* reg2_template = "x1";
                reg1 = calloc(strlen(reg1_template), sizeof(char));
                strcpy(reg1, reg1_template);
                reg2 = calloc(strlen(reg2_template), sizeof(char));
                strcpy(reg2, reg2_template);
            }
            else {
                const char* reg1_template = "x0";
                const char* reg2_template = "x1";
                reg1 = calloc(strlen(reg1_template), sizeof(char));
                strcpy(reg1, reg1_template);
                reg2 = calloc(strlen(reg2_template), sizeof(char));
                strcpy(reg2, reg2_template);
            }
            char* op_eval_output= as_op_eval(((ASTNode*)node->children->arr[0])->name, reg1, reg2);

            const char* eval_template = "%s%s%s";
            eval_output = calloc(strlen(eval_template) + strlen(right_output) + strlen(left_output) + strlen(op_eval_output) + 1, sizeof(char));


            if (is_right_binop && !is_left_binop) {
                sprintf(eval_output, eval_template, right_output, left_output, op_eval_output);
            }
            else
                sprintf(eval_output, eval_template, left_output, right_output, op_eval_output);

            break;

        case AST_VAR:
            if (node == NULL) { // idk why i have to add
                printf("error\n");
                exit(1);
            }
            char* load_var = as_var_call(node, stack_frame);

            const char* var_to_reg_template = "\t;this is var\n"
                                              "%s\tmov %s, x8\n";
            eval_output = calloc(strlen(load_var) + strlen(var_to_reg_template) + 1, sizeof(char));
            sprintf(eval_output, var_to_reg_template, load_var, reg);
            break;

        default:

            printf("Unknown op %s\n", astnode_to_string(node));
            exit(1);

    }

    return eval_output;
}

char* as_int(ASTNode* node, Stack* stack_frame) {
    char* eval_output = as_eval_expr(node->children->arr[0], NULL, stack_frame); // EVALUATE EXPRESSION
    return eval_output;
}

char* as_var_call(ASTNode* node, Stack* stack_frame) {
    if (stack_contains_symbol(stack_frame, node)) {
        ASTNode* symbol = stack_get_symbol(stack_frame, node);

        const char* load_template = "\tldr x8, [sp, #%d]\n";
        char* load_output = calloc(strlen(load_template) + 1 + 1, sizeof(char));
        sprintf(load_output, load_template, symbol->offset);

        return load_output;
    }
    printf("AS_Frontend: symbol '%s\n' not found in stack\n", node->name);
    exit(1);
    return 0;
}

char* as_var(ASTNode* node, Stack* stack_frame) {
    char* var_assignment = NULL;


    if (((ASTNode*) node->children->arr[0] )->type == AST_DEC_TYPE) {
        if (strcmp(((ASTNode*) node->children->arr[0] )->name, "int") == 0) {
            stack_push(stack_frame, node);
            var_assignment = as_int(node->children->arr[1], stack_frame);
            const char* assign_expr_template = "\tmov x8, x3\n";
            char* assign_expr = calloc(strlen(assign_expr_template) + 1, sizeof(char));
            strcpy(assign_expr, assign_expr_template);
            strcat(var_assignment, assign_expr_template);
        }
    }
    else {
        //char* load_var = as_var_call(node, stack_frame);

        var_assignment = as_int(node->children->arr[0], stack_frame);
        const char* assign_expr_template = "\tmov x8, x3\n";
        var_assignment = realloc(var_assignment, (strlen(var_assignment) + strlen(assign_expr_template)) * sizeof(char));
        //strcat(var_assignment, load_var);
        strcat(var_assignment, assign_expr_template);
    }
    const char* var_template = "%s" // Assumes var has been screated in reg x3, issues with chenging from here to the if statment above
                               "\tstr x8, [sp, #%d]\n"
                               "\tmov x8, #0\n\n"; // reset x8 for testing purposes

    char* var_output = calloc(strlen(var_assignment) + strlen(var_template) + 2, sizeof(char));
    sprintf(var_output, var_template, var_assignment, node->offset);

    return var_output;
}

char* as_func(ASTNode* node) {
    // FUTURE
    return 0;
}

char* as_built_in(ASTNode* node, Stack* stack_frame) {
    if (strcmp(node->name, "print") == 0) {// prints one value at a time, int passing no var
        //char* var_call = as_var_call(((ASTNode*)node->children)->arr[0], stack_frame);
        return built_in_print(node, stack_frame);
    }
    return NULL;
}

char* as_global(ASTNode* node, Stack* stack_frame) {
    if (node->children->num_items == 0)
        return NULL;

    char* output = calloc(1, sizeof(char));

    const char* main_template = ".global _main\n"
                              ".align 2\n"
                              "_main:\n";

    char* main_output = calloc(strlen(main_template) + 1, sizeof(char));
    strcpy(main_output, main_template);
    output = realloc(output, (strlen(main_output) + 1) * sizeof(char));
    strcat(output, main_output);

    for (int i = 0; i < (int) node->children->num_items; i++) {
        ASTNode* child = node->children->arr[i];

        char* child_output = assemble(child, stack_frame);
        output = realloc(output, (strlen(output) + strlen(child_output) + 1) * sizeof(char));
        strcat(output, child_output);
    }

    // Is this how I should end arm assembly code???
    const char* end_template = "\tmov x0, #0\n"
                               "\tmov x16, #1\n"
                               "\tsvc 0\n"
                               "ascii:\n\t.asciz \"\%d\\n\"\n"; //to hold integer that will be printed

    char* end_output = calloc(strlen(end_template) + 1, sizeof(char));
    strcpy(end_output, end_template);
    output = realloc(output, (strlen(output) + strlen(end_output) + 1) * sizeof(char));
    strcat(output, end_output);


    return output;
}
