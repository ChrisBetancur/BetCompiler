#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

// read 
char* read_file(char* file_path) {
    FILE* file;
    char* line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;


    file = fopen(file_path, "rb");

    if (file == NULL) {
        printf("Error: file not found at %s", file_path);
        exit(1);
    }

    line_size = getline(&line_buf, &line_buf_size, file);

    char* buffer = (char*) calloc(1, sizeof(char));

    while (line_size >= 0) {
        buffer = (char*) realloc(buffer, (strlen(buffer) + strlen(line_buf) + 1) * sizeof(char));
        strcat(buffer, line_buf);
        line_size = getline(&line_buf, &line_buf_size, file);
    }
    
    free(line_buf);
    line_buf = NULL;

    fclose(file);

    //printf("\n---Buffer---\n%s\n", buffer);

    return buffer;
}
