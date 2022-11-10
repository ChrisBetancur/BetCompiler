#ifndef IO_H
#define IO_H

char* read_file(char* file_path);

char* write_file(char* file_path, char* buffer);

char* sh(const char* cmd);

#endif
