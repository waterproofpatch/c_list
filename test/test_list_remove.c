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

/**
 * @brief test that we can remove the head of the list
 * */
void test_list_remove_head()
{
    list_t list     = {0};
    list.head       = &g_node[0];
    list.tail       = &g_node[1];
    list.tail->next = NULL;
    list.head->next = list.tail;
    list.count      = 2;
    list.list_free  = stub_list_free;

    g_expected = &g_node[0]; /* the head */
    TEST_ASSERT_EQUAL(1, list_remove(&list, g_node[0].element));
    TEST_ASSERT_EQUAL(1, list.count);
    TEST_ASSERT_EQUAL(list.head, list.tail);
    TEST_ASSERT_EQUAL(0xfeedface, list.tail->element);
    TEST_ASSERT_EQUAL(0xfeedface, list.head->element);
    TEST_ASSERT_EQUAL(1, g_num_frees);
}

/**
 * @brief test that we can remove an item in the middle of the list
 * */
void test_list_remove_middle()
{
    list_node_t node_1 = {0};
    list_node_t node_2 = {0};
    list_node_t node_3 = {0};
    list_t      list   = {0};

    node_1.element = (void *)1;
    node_1.next    = &node_2;
    node_2.element = (void *)2;
    node_2.next    = &node_3;
    node_3.element = (void *)3;
    node_3.next    = NULL;

    list.head      = &node_1;
    list.tail      = &node_3;
    list.count     = 3;
    list.list_free = stub_list_free;

    g_expected = &node_2; /* the middle */
    TEST_ASSERT_EQUAL(1, list_remove(&list, node_2.element));
    TEST_ASSERT_EQUAL(2, list.count);
    TEST_ASSERT_EQUAL(&node_1, list.head);
    TEST_ASSERT_EQUAL(&node_3, list.tail);
    TEST_ASSERT_EQUAL(1, g_num_frees);
}