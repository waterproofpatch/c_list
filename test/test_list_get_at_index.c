#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

/* code under test */
#include "list.h"

void setUp()
{
}

void tearDown()
{
}

void test_list_get_at_index_success()
{
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_t      list    = {.head = &entry_1, .tail = &entry_1, .count = 1};
    TEST_ASSERT_EQUAL(entry_1.element, list_get_at_index(&list, 0));
}

void test_list_get_at_index_not_found()
{
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_t      list    = {.head = &entry_1, .tail = &entry_1, .count = 1};
    TEST_ASSERT_EQUAL(NULL, list_get_at_index(&list, 1));
}

void test_list_get_at_index_second_entry()
{
    list_node_t entry_2 = {.element = (void *)0xf00dface, .next = NULL};
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = &entry_2};
    list_t      list    = {.head = &entry_1, .tail = &entry_2, .count = 1};
    TEST_ASSERT_EQUAL(entry_2.element, list_get_at_index(&list, 1));
}

void test_list_get_at_index_fail_invalid_list()
{
    TEST_ASSERT_EQUAL(NULL, list_get_at_index(NULL, 1));
}