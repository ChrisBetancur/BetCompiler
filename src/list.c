#include "include/list.h"
#include "include/ast_node.h"
/*
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
    printf("%d %d\n", list_struct->arr_size, list_struct->arr_count);
    if (list_struct->arr_size == list_struct->arr_count) {
        list_struct->arr_size += 2;
        realloc(list_struct->arr, list_struct->arr_size * list_struct->type_size);
    }

    if (data_size != list_struct->type_size) {
        printf("List: Incorrect type size");
        return;
    }

    list_struct->arr[list_struct->arr_count] = &data;
    printf("%s\n", ((ASTNode*)data)->name);
    list_struct->arr_count++;
}*/

List* init_list(size_t data_size){
  List* list_struct = calloc(1, sizeof(struct LIST_STRUCT));
  list_struct->arr_size = 0;
  list_struct->data_size = data_size;
  list_struct->arr = 0;
  list_struct->num_items = 0;

  return list_struct;
}

void list_append(List* list_struct, void* data, size_t data_size)
{
  list_struct->arr_size += 1;

  if (!list_struct->arr)
    list_struct->arr = calloc(1, list_struct->arr_size);
  else
    list_struct->arr = realloc(list_struct->arr, (list_struct->arr_size * list_struct->data_size));

    list_struct->arr[list_struct->arr_size-1] = data;
    list_struct->num_items++;
}


void print_list(List* list) {
    for (int i = 0; i < list->num_items; i++) {
        printf("%p\n", list->arr[i]);
        printf("hello\n");
    }
}




