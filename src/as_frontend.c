#include "include/as_frontend.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char* assemble(ASTNode* node) {
    char* output = calloc(1, sizeof(char));
    char* next_val = 0;

    switch (node->type) {
        case AST_GLOBAL:
            next_val = as_global(node);
            break;
        case AST_INT:
            next_val = as_int(node);
            break;

        case AST_VAR:
            next_val = as_var(node);
            break;

        default:
            printf("Assembler Front End: '%s\n' unknown Abstract Syntax Tree node type", astnode_type_to_string(node->type));
            exit(1);
    }

    output = realloc(output, (strlen(next_val) + 1) * sizeof(char));
    strcat(output, next_val);

    return output;
}

char* as_op_eval(char* op, char* reg1, char* reg2) { // only add and sub
    if (strcmp(op, "+") == 0) {
        const char* sum_template = "\tadd x3, %s, %s\n";
        char* sum_output = calloc(strlen(sum_template) + strlen(reg1) + strlen(reg2) + 1, sizeof(char));
        sprintf(sum_output, sum_template, reg1, reg2);
        printf("hello %s\n", reg1);
        printf("%s\n", sum_output);
        return sum_output;
    }
    return NULL;
}


char* as_eval_expr(ASTNode* node, char* reg) { // evalutes ints not including calling vars in expressions yet
    char* eval_output = NULL;


    bool is_right_binop = false;
    bool is_left_binop = false;

    char* right_output = NULL;
    char* left_output = NULL;

    switch (node->type) { // check if AST_TERM is unused
        case AST_INT:
            if (reg != NULL) { //??? if i takr out if statement defining const string does not work
                const char* int_template = "\tmov %s, #%s\n";
                eval_output = calloc(strlen(int_template) + strlen(reg) + strlen(node->name), sizeof(char));
                sprintf(eval_output, int_template, reg, node->name);
            }
            break;

        case AST_BINARY_OP:
            if (((ASTNode*) node->children->arr[1])->type == AST_BINARY_OP) {
                is_right_binop = true;
                right_output = as_eval_expr(node->children->arr[1], "x1");
            }
            else {
                const char* right_template = "\tmov x1, #%s\n";
                char* value = ((ASTNode*) node->children->arr[1])->name;
                right_output = calloc(strlen(right_template) + strlen(value) + 1, sizeof(char));
                sprintf(right_output, right_template, value);
            }

            if (((ASTNode*) node->children->arr[2])->type== AST_BINARY_OP) {
                left_output = as_eval_expr(node->children->arr[2], "x0");
                is_left_binop = true;
            }
            else {
                const char* left_template = "\tmov x0, #%s\n";
                char* value = ((ASTNode*) node->children->arr[2])->name;
                left_output = calloc(strlen(left_template) + strlen(value) + 1, sizeof(char));
                sprintf(left_output, left_template, value);
            }
            printf("left %d %s\n", is_right_binop, left_output);
            printf("right %d %s\n", is_left_binop, right_output);
            //printf("hello\n");

            char* reg1;
            char* reg2;


            if (is_left_binop) {
                printf("hELLO\n");
                const char* reg1_template = "x3";
                const char* reg2_template = "x1";
                reg1 = calloc(strlen(reg1_template), sizeof(char));
                strcpy(reg1, reg1_template);
                reg2 = calloc(strlen(reg2_template), sizeof(char));
                strcpy(reg2, reg2_template);
            }
            else if (is_right_binop) {
                printf("hELLO\n");
                const char* reg1_template = "x3";
                const char* reg2_template = "x0";
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
                printf("Hello\n");
                sprintf(eval_output, eval_template, right_output, left_output, op_eval_output);
            }
            else
                sprintf(eval_output, eval_template, left_output, right_output, op_eval_output);


            break;

        default:
            //printf("Hello\n");

            printf("Unknown op %s\n", astnode_to_string(node));

    }

    printf("%s\n", eval_output);
    return eval_output;
}

char* as_int(ASTNode* node) {
    char* eval_output = as_eval_expr(node->children->arr[0], NULL); // EVALUATE EXPRESSION
    //const char* set_int_template = "\tmov x8, %s\n";
    /*char* set_int = calloc(strlen(eval_output) + strlen(((ASTNode*) node->children->arr[0])->name) + 1, sizeof(char));*/
    //sprintf(set_int, eval_output, ((ASTNode*) node->children->arr[0])->name);
    return eval_output;
}

char* as_var(ASTNode* node) {
    char* var_assignment = NULL;

    if (((ASTNode*) node->children->arr[0] )->type == AST_DEC_TYPE) {
        if (strcmp(((ASTNode*) node->children->arr[0] )->name, "int") == 0) {
            var_assignment = as_int(node->children->arr[1]);
            const char* assign_expr_template = "\tmov x8, x3\n";
            char* assign_expr = calloc(strlen(assign_expr_template), sizeof(char));
            strcpy(assign_expr, assign_expr_template);
            strcat(var_assignment, assign_expr);
        }
    }

    node->offset = 0x4; // Find way to increment offset for each var stored

    const char* var_template = "%s"
                               "\tstr x8, [sp, #%x]\n";

    char* var_output = calloc(strlen(var_assignment) + strlen(var_template) + 2, sizeof(char));
    sprintf(var_output, var_template, var_assignment, node->offset);

    //__asm__ ("mov x0, #2");

    return var_output;
}

char* as_func(ASTNode* node) {
    // FUTURE
    return 0;
}

char* as_global(ASTNode* node) {
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

        char* child_output = assemble(child);
        output = realloc(output, (strlen(output) + strlen(child_output) + 1) * sizeof(char));
        strcat(output, child_output);
    }

    // Is this how I should end arm assembly code???
    const char* end_template = "\tmov x0, #0\n"
                               "\tmov x16, #1\n"
                               "\tsvc 0\n";

    char* end_output = calloc(strlen(end_template) + 1, sizeof(char));
    strcpy(end_output, end_template);
    output = realloc(output, (strlen(output) + strlen(end_output) + 1) * sizeof(char));
    strcat(output, end_output);


    return output;
}
