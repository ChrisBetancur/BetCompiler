#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

/*
 * Function: read_file
 *
 * reads file from file path and puts it in string
 *
 * file_path: file path of file
 *
 * returns: buffer containing the file contents
 */
char* read_file(char* file_path) {
    FILE* file;
    char* line_buf = NULL;
    size_t line_buf_size = 0;

    ssize_t line_size;


    file = fopen(file_path, "rb");

    if (file == NULL) {
        printf("Error: file not found at %s\n", file_path);
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

    return buffer;
}


void write_file(char* file_path, char* buffer) {
    FILE* file;

    file = fopen(file_path, "wb");

    if (file == NULL) {
        printf("Error: file not found at %s", file_path);
        exit(1);
    }

    fputs(buffer, file);
    fclose(file);
}



char* sh(const char* cmd) {
    char* output = (char*) calloc(1, sizeof(char));
    output[0] = '\0';

    FILE *fp;
    char path[1035];

    fp = popen(cmd, "r");

    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        output = (char*) realloc(output, (strlen(output) + strlen(path) + 1) * sizeof(char));
        strcat(output, path);
    }

    pclose(fp);

  return output;
}
