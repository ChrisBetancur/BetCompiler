#include <stdio.h>
#include <stdlib.h>

#ifndef NODE_H
#define NODE_H

typedef struct NODE_STRUCT {
    void* data;
    size_t data_size;
    struct NODE_STRUCT* next;
    struct NODE_STRUCT* prev;
} Node;

Node* init_node(void* data, size_t data_size);

#endif
