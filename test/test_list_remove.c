#include <stddef.h>
#include <stdlib.h>
#include "unity.h"

// mocks

// code under test
#include "list.h"

// globals
static list_node_t g_node[] = {
    {.next = &g_node[1], .element = (void *)0xdeadbeef},
    {.next = NULL, .element = (void *)0xfeedface}};
static int   g_num_frees = 0;
static void *g_expected  = NULL;

void setUp()
{
    g_num_frees = 0;
}

void tearDown()
{
}

void stub_list_free(void *ptr)
{
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_NOT_NULL(g_expected);
    TEST_ASSERT_EQUAL(ptr, g_expected);
    g_num_frees++;
}

/**
 * @brief test that we can remove the tail of the list
 * */
void test_list_remove_tail()
{
    list_t list     = {0};
    list.head       = &g_node[0];
    list.tail       = &g_node[1];
    list.tail->next = NULL;
    list.head->next = list.tail;
    list.count      = 2;
    list.list_free  = stub_list_free;

    g_expected = &g_node[1]; /* the tail */
    TEST_ASSERT_EQUAL(1, list_remove(&list, list.tail->element));
    TEST_ASSERT_EQUAL(1, list.count);
    TEST_ASSERT_EQUAL(list.head, list.tail);
    TEST_ASSERT_EQUAL(0xdeadbeef, list.tail->element);
    TEST_ASSERT_EQUAL(0xdeadbeef, list.head->element);
    TEST_ASSERT_EQUAL(1, g_num_frees);
}