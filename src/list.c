#include "include/list.h"
#include "include/ast_node.h"

List* init_list(size_t data_size){
  List* list_struct = calloc(1, sizeof(struct LIST_STRUCT));
  list_struct->data_size = data_size;
  list_struct->arr = 0;
  list_struct->num_items = 0;

  return list_struct;
}


void list_append(List* list_struct, void* data, size_t data_size) {
    if (list_struct->data_size != data_size) {
        printf("Invalid data\n");
        return;
    }

    
    list_struct->num_items += 1;

    if (list_struct->arr == 0) {
        list_struct->arr = malloc(sizeof(list_struct->data_size));
        list_struct->arr[0] = data;
        return;
    }

    void** new_arr = malloc(list_struct->num_items * sizeof(list_struct->data_size));

    for (int i = 0; i < list_struct->num_items; i++) {
        new_arr[i] = list_struct->arr[i];
    }

    free(list_struct->arr);


    list_struct->arr = new_arr;
    list_struct->arr[list_struct->num_items - 1] = data;
}

/*
void list_append(List* list_struct, void* data, size_t data_size)
{
    if (list_struct->data_size != data_size) {
        printf("Invalid data\n");
        return;
    }

    list_struct->num_items += 1;
    if (list_struct->arr == 0) {
        list_struct->arr = calloc(1, list_struct->data_size);
    }
    else
        list_struct->arr = realloc(list_struct->arr, (list_struct->num_items * list_struct->data_size));

    list_struct->arr[list_struct->num_items-1] = data;
}
*/

void print_list(List* list) {
    for (int i = 0; i < list->num_items; i++) {
        printf("%d\n", list->arr[i]);
    }
}




