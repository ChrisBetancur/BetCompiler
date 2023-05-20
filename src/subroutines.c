#include "include/subroutines.h"
#include "include/asm.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* divide(char* reg_src, char* reg_dest1, char* reg_dest2) {
    /*const char* div_template =  "\ndiv:\n\tsub x4, %s, #1\n"
                                "loop:\n"
                                     "\tadd %s, %s, #1\n"
                                     "\tsub %s, %s, %s\n"
                                     "\tcmp %s, x4\n"
                                     "\tbgt loop\n\n"
                                "\t;store remainder\n\tmov x9, x0\n\tret\n\n";*/

    const char* div_template =  "\n\n\tsub x4, %s, #1\n"
                                "div:\n"
                                     "\tadd %s, %s, #1\n"
                                     "\tsub %s, %s, %s\n"
                                     "\tcmp %s, x4\n"
                                     "\tbgt div\n\n"
                                "\t;store remainder\n\tmov x9, x0\n\n\n";

    char* div_output = calloc(strlen(div_template) + strlen(reg_dest1) + strlen(reg_src) + strlen(reg_src) + strlen(reg_dest1) + strlen(reg_dest1) + strlen(reg_dest2) + strlen(reg_dest1), sizeof(char));
    sprintf(div_output, div_template, reg_dest2, reg_src, reg_src, reg_dest1, reg_dest1, reg_dest2, reg_dest1);

    /*char* div_output = calloc(strlen(div_template) + strlen(OP_REG1) * 4 + strlen(RESULT_REG) * 2 + strlen(OP_REG2), sizeof(char));

    sprintf(div_output, div_template, OP_REG2, RESULT_REG, RESULT_REG, OP_REG1, OP_REG1, OP_REG2, OP_REG1);*/


    return div_output;
}
