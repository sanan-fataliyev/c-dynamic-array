#include <stdio.h>
#include <limits.h>
#include "list.h"


void fail(char* message)
{
        printf("FAIL: %s\n", message);
}

void assert(int condition, char* message)
{
        if(!condition)
                fail(message);
}

void warn(char* message)
{
        printf("WARNING: %s\n", message);
}

void assert_lists_eq(List *ptr_l1, List *ptr_l2)
{
        if(ptr_l1 == ptr_l2)
        {
                warn("identical pointers");
                return;
        }
        
        assert(ptr_l1->count == ptr_l2->count, "items count missmatch");
        assert(ptr_l1->capacity == ptr_l2->capacity,"list capacity missmatch");
        
        for(int i = 0; i < ptr_l1->count; i++)
        {
                assert(ptr_l1->ptr_array[i] == ptr_l2->ptr_array[i], "items missmatch");
        }
}

void test_list_create()
{
        printf("TEST create_list\n");
        List l = list_create();

        assert(l.count == 0, "count is not zero");
        assert(l.capacity == 16, "init capasiti is not 16");
        assert(l.ptr_array != NULL, "internal arrar is null");
        _list_free(&l);
}

void test_list_add()
{
        printf("TEST list_add\n");
        List l = list_create();

        list_add(&l, 5);

        assert(l.count==1, "increament count faled");
        assert(l.ptr_array[0] == 5, "added item not found");

        //fill capacity
        while(l.count < l.capacity)
        {
                list_add(&l, 10);
        }

        assert(l.capacity == l.count, "early resize");

        list_add(&l, 5);

        assert(l.capacity == 32, "internal array is not doubled");

        _list_free(&l); 
}

void test_list_get()
{
        printf("TEST list_get\n");

        int items[6] = {22, -99, INT_MIN, INT_MAX, 0, 1};
        
        List l = list_create();
        
        for(int i = 0; i < 6; i++)
        {
                list_add(&l, items[i]);
        }

        for(int i = 0; i < 6; i++)
        {
                assert(list_get(&l, i) == items[i], "wrong element returned");
        }

        List l2 = list_create();

        for(int i = 0; i < 1000000; i++)
        {
                list_add(&l2, i);
        }

        for(int i = 0; i < 1000000; i++)
        {
                assert(list_get(&l2, i) == i, "wrong element return");
        }
        _list_free(&l);
        _list_free(&l2);
}

void test_list_insert()
{
        printf("TEST list_insert\n");

        List l = list_create();
        
        list_insert(&l, 5, 0);// {5}

        assert(l.count == 1 && l.ptr_array[0] == 5, "insert to empty list");

       list_add(&l, 6);
       list_add(&l, 7);
       list_add(&l, 8);
       list_add(&l, 9);
       //{5,6,7,8,9}

       list_insert(&l, 4, 0);
       //{4,5,6,7,8,9}

       assert(l.ptr_array[0] == 4, "insert front");
       assert(l.ptr_array[1] == 5, "shift left items");
       assert(l.count == 6 && l.ptr_array[5] == 9, "grow");

       list_insert(&l, 10, l.count);// insert end
       //{4,5,6,7,8,9,10}

       assert(l.count == 7 && l.ptr_array[6] == 10, "insert end");

       list_insert(&l, 666, 3);
        //{4,5,6,666,7,8,9,10}

        assert(l.ptr_array[2] == 6, "insert middle");
        assert(l.ptr_array[3] == 666, "insert middle");
        assert(l.ptr_array[4] == 7, "insert middle");
        
        _list_free(&l); 
}

void test_list_delete()
{
        printf("TEST list_delete\n");

        List l = list_create();

        for(int i = 0; i < 10; i++)
        {
                list_add(&l, i);
        }
        // {0,1,2,3,4,5,6,7,8,9}

        //delete from front
        list_delete(&l, 0);
        //{1,2,3,4,5,6,7,8,9}
        
        assert(
                l.count == 9 &&
                list_get(&l, 0) == 1 &&
                list_get(&l,l.count-1) == 9,
                "remove the first item");


        //delete last item
        list_delete(&l, l.count-1);
        // {1,2,3,4,5,6,7,8}


        assert(list_get(&l, l.count-1) == 8,
        "remove last item");

        list_delete(&l, 3);
        // {1,2,3,5,6,7,8}

        assert(
                list_get(&l, 2) == 3 &&
                list_get(&l, 3) == 5, 
                "remove item from middle");

        
        List l2 = list_create();

        for(int i = 0; i < 33; i++)
        {
                list_add(&l2, i);
        }
        //{0,1,2...31,32}, count=33, capacity: 64

        assert(l2.capacity == 64, "incorrect capacity");
        
        while(l2.count > 16)
        {
                list_delete(&l2, 0);
        }

        assert(l2.count == 16 && l2.capacity == 32, "resize capacity to half when elements count less than or equal to 1/4 of the capacity");

        _list_free(&l);
        _list_free(&l2);
}

void test_list_pop()
{
        printf("TEST list_pop\n");

        List l = list_create();

        for(int i = 0; i <= 100; i++)
        {
                list_add(&l, i);
        }

        for(int i = 100; i >= 0; i--)
        {
                int last_item = list_pop(&l);

                assert(last_item == i, "wrong item poped");
                assert(l.count == i, "not removed");
        }


        _list_free(&l);
}

void test_list_remove()
{
        printf("TEST list_remove\n");

        List l = list_create();

        for(int i = 0; i < 100; i ++)
        {
                list_add(&l, 10 + 5*(i%2));
        }
        //{10, 15, 10, 15, ....} count: 100


        int removed_element_count = list_remove(&l, 10);
        //{15,15,15,...} count: 50

        assert(l.count = 50, "decrement count");
        assert(removed_element_count == 50, "return removed items count");
        assert(list_remove(&l, 10) == 0, "all removed");

        list_remove(&l, 15);
        //{} count: 0

        assert(l.count == 0, "remove until empty");

        _list_free(&l);
}

void test_list_index_of()
{
        printf("TEST list_index_of\n");

        List l = list_create();

        assert(list_index_of(&l, 5) == -1, "search in empty list");

        list_add(&l, 5);
        list_add(&l, -1);
        list_add(&l, 15);
        list_add(&l, 5);

        assert(
                list_index_of(&l, 5) == 0 &&
                list_index_of(&l,-1) == 1 &&
                list_index_of(&l, 15) == 2,
                "search existing item");

        assert(
                list_index_of(&l, 99) == -1,
                "search nonexisting item");

        _list_free(&l);
}
