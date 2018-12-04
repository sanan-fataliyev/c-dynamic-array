#ifndef DYNAMIC_LIST
#define DYNAMIC_LIST

typedef struct {
        int count;
        int capacity;
        int *ptr_array;
} List;


List list_create();
void list_add(List *ptr_l, int item);
int list_get(List *ptr_l, int index);
void list_insert(List *ptr_l, int item, int index);
void list_prepend(List *ptr_l, int item);
int list_pop(List *ptr_l);
int list_delete(List *ptr_l, int index);
int list_remove(List *ptr_l, int item);
int list_index_of(List *ptr_l, int item);

void list_print_items(List *ptr_l, char *seperator);
void list_to_string(List *ptr_l);

void _list_validate_index(List *ptr_l, int index);
void _list_set_capacity(List *ptr_l, int capacity);
void _list_double_capacity_if_full(List *ptr_l);
void _list_half_capacity_if_small(List *ptr_l);
void _list_shift(List *ptr_l, int index, int rightOrLeft);
void _list_throw_error(List *ptr_l, char *reason);
void _list_free(List *ptr_l);

#endif