#ifndef IO_H
#define IO_H

char* read_file(char* file_path);

void write_file(char* file_path, char* buffer);

char* sh(const char* cmd);

#endif
