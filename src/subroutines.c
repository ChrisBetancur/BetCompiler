#include "include/subroutines.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* divide(char* reg_src, char* reg_dest1, char* reg_dest2) {
    const char* div_template =  "\n\t;divide\n\tsub x4, %s, #1\n"
                                "div:\n"
                                     "\tadd %s, %s, #1\n"
                                     "\tsub %s, %s, %s\n"
                                     "\tcmp %s, x4\n"
                                     "\tbgt div\n\n"
                                "\t;store remainder\n\tmov x9, x0\n\n";

    char* div_output = calloc(strlen(div_template) + strlen(reg_dest1) + strlen(reg_src) + strlen(reg_src) + strlen(reg_dest1) + strlen(reg_dest1) + strlen(reg_dest2) + strlen(reg_dest1), sizeof(char));
    sprintf(div_output, div_template, reg_dest2, reg_src, reg_src, reg_dest1, reg_dest1, reg_dest2, reg_dest1);

    return div_output;
}
