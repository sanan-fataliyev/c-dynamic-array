## Dynamic Array aka ArrayList aka Vector  data structure in C

probably there are tons of other implementations which are more efficient and c-*styled.*  i'm not a C geek.

```clang
        //some example usages

        List l = list_create();

        list_add(&l, 10);
        list_add(&l, 25);
        list_add(&l, 30);
        list_add(&l, 50);
        // count: 4, capacity: 16, items: 10,25,30,50
        
        int num = list_get(&l, 2);
        // 30

        list_insert(&l, 15, 1);
        // count: 5, capacity: 16, items: 10,15,25,30,50

        list_delete(&l, 3);
        // count: 4, capacity: 8, items: 10,15,25,50


        int index = list_index_of(&l, 50);
        // 3
        
        int index2 = list_index_of(&l, 18);
        // -1
        
        int last =  list_pop(&l);
        // last: 50, count: 3, capacity: 8, items: 10,15,25
        

        // free allocated memory
        _list_free(&l);
```
