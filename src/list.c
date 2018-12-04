#include <stdio.h>
#include <stdlib.h>
#include "list.h"


//initializes new list with 16 capacity
List list_create()
{
        List l;
        l.count = 0;
        l.ptr_array = NULL;
        _list_set_capacity(&l, 0x10);
        return l;
}

//adds item to the end of the list
void list_add(List *ptr_l, int item)
{
        _list_double_capacity_if_full(ptr_l);
        int *target = ptr_l->ptr_array + ptr_l->count;
        *target = item;
        ptr_l->count++;
}


//returns item at given index, blows up if index out of bounds
int list_get(List *ptr_l, int index)
{
        _list_validate_index(ptr_l, index);
        return *(ptr_l->ptr_array + index);
}


// inserts item at index, shifts that index’s value and trailing elements to the right
void list_insert(List *ptr_l, int item, int index)
{
        if (index == ptr_l->count)
        {
                list_add(ptr_l, item);
        }
        else
        {
                _list_validate_index(ptr_l, index);
                _list_double_capacity_if_full(ptr_l);

                for (int i = ptr_l->count; i > index; --i)
                {
                        *(ptr_l->ptr_array + i) = *(ptr_l->ptr_array + i - 1);
                }
                *(ptr_l->ptr_array+index) = item;
                ptr_l->count++;
        }
}


 //removes from end, return value
int list_pop(List *ptr_l)
{
        if(ptr_l->count == 0)
        {
                _list_throw_error(ptr_l, "no elements left");
        }
        int last_index = ptr_l->count-1;
        int last_item = ptr_l->ptr_array[last_index];
        ptr_l->count--;
        _list_half_capacity_if_small(ptr_l);
        return last_item;
}


//deletes item at index, shifting all trailing elements left
int list_delete(List *ptr_l, int index)
{
        if(index == ptr_l->count-1)
        {
                return list_pop(ptr_l);
        }

        _list_validate_index(ptr_l, index);
        _list_shift(ptr_l, index, -1);
        ptr_l->count--;
        _list_half_capacity_if_small(ptr_l);
}


//looks for value and removes (even if in multiple places) and returns their count
int list_remove(List *ptr_l, int item)
{
        if(ptr_l->count == 0)
        {
                return 0;
        }

        int count = 0;

        for(int i = 0; i < ptr_l->count; i++)
        {
                if(ptr_l->ptr_array[i] == item)
                {
                        list_delete(ptr_l, i--);
                        count++;
                }
        }
        return count;
}

//looks for item and returns first index with that value, -1 if not found
int list_index_of(List *ptr_l, int item)
{
        for(int i = 0; i < ptr_l->count; i++)
        {
                if(ptr_l->ptr_array[i] == item)
                {
                        return i;
                }
        }
        return -1;
}




//resizes internal array
void _list_set_capacity(List *ptr_l, int new_capacity)
{
        if(new_capacity < ptr_l->count)
        {
                _list_throw_error(ptr_l, "new capacity is too small to keep existing items");
        }

        ptr_l->capacity = new_capacity;

        void *ptr_new_array = realloc(ptr_l->ptr_array, ptr_l->capacity *  sizeof(int));

        if(ptr_new_array == NULL)
        {
                _list_throw_error(ptr_l, "memeory allocation failed");
        }

        ptr_l->ptr_array = ptr_new_array;
}


//prints error reason, frees memory, exits
void _list_throw_error(List *ptr_l, char *reason)
{
        printf("error: %s\n", reason);
        _list_free(ptr_l);
        exit(1);
}

void _list_free(List *ptr_l)
{
        free(ptr_l->ptr_array);
}


// shifts array elements by 1 starting from given index 
// rightOrLeft param determines the direction: 1 - left, -1 right
// [0,1,2,3,4]  2, 1 -> [0,1,2,2,3,4,5]
// [0,1,2,3,4]  2,-1 -> [0,1,2,4,5,5]
void _list_shift(List *ptr_l, int index, int rightOrLeft)
{
        int hand = ptr_l->ptr_array[index+1];
        
        for(int i = index-rightOrLeft; i < ptr_l->count; i++)
        {
                int dest_i = i + rightOrLeft;

                if(i < index)
                {
                        hand = ptr_l->ptr_array[i+1];
                        continue;
                }
                int temp = ptr_l->ptr_array[i+1];
                ptr_l->ptr_array[dest_i] = hand;
                hand = temp;
        }
}


void list_print_items(List *ptr_l, char *seperator)
{
        if (ptr_l->count > 0)
        {
                int *address =  ptr_l->ptr_array;
                int *stop_address = address + ptr_l->count;

                //first item
                printf("%d", *address);

                while (++address < stop_address)
                {
                        //seperator + current item
                        printf("%s%d", seperator, *address);
                }
        }
        else
        {
                printf("<emtpy>");
        }
}


void list_to_string(List *ptr_l)
{
        printf("count: %d, capacity: %d, items: ", ptr_l->count, ptr_l->capacity);
        list_print_items(ptr_l, ",");
        printf("\n");
}

void _list_validate_index(List *ptr_l, int index)
{
        if(index < 0 || index >= ptr_l->count) 
        {
                _list_throw_error(ptr_l, "index out of range");
        }
}

void _list_double_capacity_if_full(List *ptr_l)
{
        if(ptr_l->count == ptr_l->capacity)
        {
                _list_set_capacity(ptr_l, ptr_l-> capacity << 1);
        }
}

void _list_half_capacity_if_small(List *ptr_l)
{
        int quarter = ptr_l -> capacity >> 2;

        if(quarter != 0 && ptr_l->count <= quarter)
        {
                _list_set_capacity(ptr_l, ptr_l->capacity >> 1);
        }
}
