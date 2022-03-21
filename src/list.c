#include "include/list.h"

const int DEFAULT_SIZE = 10;


List* init_list(size_t type_size) {
    List* list = malloc(sizeof(struct LIST_STRUCT));

    list->type_size = type_size;
    list->arr_size = DEFAULT_SIZE;
    list->arr_count = 0;

    list->arr = malloc(list->arr_size * type_size);

    return list;
}

void list_append(List* list_struct, void* data, size_t data_size) {
    if (list_struct->arr_size == list_struct->arr_count) {
        list_struct->arr_size += 2;
        realloc(list_struct->arr, list_struct->arr_size * list_struct->type_size);
    }

    if (data_size != list_struct->type_size) {
        printf("List: Incorrect type size");
        return;
    }

    list_struct->arr[list_struct->arr_count] = data;
    list_struct->arr_count++;
}


void print_list(List* list_struct) {
    for (int i = 0; i < list_struct->arr_count; i++) {
        printf("%p\n", list_struct->arr[i]);
    }
}


