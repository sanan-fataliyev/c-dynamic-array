#ifndef DYNAMIC_LIST_TESTS
#define DYNAMIC_LIST_TESTS

#include "list.h"

void fail(char* message);
void fail_if(int condition, char* message);
void warn(char* message);
void assert_lists_eq(List *ptr_l1, List *ptr_l2);
void test_list_create();
void test_list_add();
void test_list_get();
void test_list_insert();
void test_list_delete();
void test_list_pop();
void test_list_remove();
int test_list_index_of();

#endif