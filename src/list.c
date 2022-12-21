#include "include/list.h"
#include "include/ast_node.h"

/*
 * Function: init_list
 *
 * Creates list to store values
 *
 * data_size: size of the kind of data the list will store
 *
 * returns: list ref
 */

List* init_list(size_t data_size){
  List* list_struct = calloc(1, sizeof(struct LIST_STRUCT));
  list_struct->data_size = data_size;
  list_struct->arr = 0;
  list_struct->num_items = 0;

  return list_struct;
}

/*
 * Function: list_append
 *
 * Appends data to list
 *
 * list_struct: list used to append data too
 * data: data used to append
 * data_size: size of the data used to append
 */

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
