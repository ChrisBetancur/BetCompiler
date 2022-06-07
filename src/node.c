#include <string.h>

#include "include/node.h"

Node* init_node(void* data, size_t data_size) {
    Node* node = malloc(sizeof(struct NODE_STRUCT));

    node->data = malloc(data_size);
    node->data_size = data_size;
   
    node->data = data;
    
    node->next = NULL;
    node->prev = NULL;

    return node;
}
