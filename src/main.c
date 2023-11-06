#include <stdio.h>
#include <time.h>

#include "include/bet_compiler.h"
#include "include/node.h"
#include "include/symbol_table.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    clock_t begin = clock();
    bet_compile(argv[1]);

    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Compilation time: %lf\n", time_spent);
}
