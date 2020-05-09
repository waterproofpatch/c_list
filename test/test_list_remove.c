#include <stddef.h>
#include <stdlib.h>
#include "unity.h"

// mocks

/* code under test */
#include "list.h"

// globals
static int   g_num_frees = 0;
static void *g_expected  = NULL;

void setUp()
{
    g_num_frees = 0;
    g_expected  = NULL;
}

void tearDown()
{
}

void stub_list_free(void *ptr)
{
    TEST_ASSERT_NOT_NULL(ptr);
    TEST_ASSERT_EQUAL(g_expected, ptr);
    g_num_frees++;
}

/**
 * @brief test that we can remove the tail of the list
 * */
void test_list_remove_tail()
{
    list_node_t node_1 = {0};
    list_node_t node_2 = {0};
    list_t      list   = {0};

    node_1.element = (void *)1;
    node_1.next    = &node_2;
    node_2.element = (void *)2;
    node_2.next    = NULL;

    list.head      = &node_1;
    list.tail      = &node_2;
    list.count     = 2;
    list.list_free = stub_list_free;

    g_expected = &node_2;
    TEST_ASSERT_EQUAL(1, list_remove(&list, node_2.element));
    TEST_ASSERT_EQUAL(1, list.count);
    TEST_ASSERT_EQUAL(list.head, list.tail);
    TEST_ASSERT_EQUAL((void *)1, list.head->element);
    TEST_ASSERT_EQUAL(1, g_num_frees);
}

/**
 * @brief test that we can remove the head of the list
 * */
void test_list_remove_head()
{
    list_node_t node_1 = {0};
    list_node_t node_2 = {0};
    list_t      list   = {0};

    node_1.element = (void *)1;
    node_1.next    = &node_2;
    node_2.element = (void *)2;
    node_2.next    = NULL;

    list.head      = &node_1;
    list.tail      = &node_2;
    list.count     = 2;
    list.list_free = stub_list_free;

    g_expected = &node_1;
    TEST_ASSERT_EQUAL(1, list_remove(&list, node_1.element));
    TEST_ASSERT_EQUAL(1, list.count);
    TEST_ASSERT_EQUAL(list.head, list.tail);
    TEST_ASSERT_EQUAL((void *)2, list.head->element);
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

/**
 * @brief test that we can remove an item in the middle of the list
 * */
void test_list_remove_does_not_exist()
{
    list_node_t node_1 = {0};
    list_node_t node_2 = {0};
    list_node_t node_3 = {0};
    list_t      list   = {0};

    node_1.element = (void *)1;
    node_1.next    = &node_3;
    node_2.element = (void *)2;
    node_2.next    = NULL;
    node_3.element = (void *)3;
    node_3.next    = NULL;

    list.head      = &node_1;
    list.tail      = &node_3;
    list.count     = 2;
    list.list_free = stub_list_free;

    TEST_ASSERT_EQUAL(0, list_remove(&list, node_2.element));
    TEST_ASSERT_EQUAL(2, list.count);
    TEST_ASSERT_EQUAL(&node_1, list.head);
    TEST_ASSERT_EQUAL(&node_3, list.tail);
    TEST_ASSERT_EQUAL(0, g_num_frees);
}

/**
 * @brief test that the list handles invalid arguments
 * */
void test_list_remove_fail_invalid_element()
{
    TEST_ASSERT_EQUAL(0, list_remove((list_t *)0xdeadbeef, NULL));
}

/**
 * @brief test that the list handles invalid arguments
 * */
void test_list_remove_fail_invalid_list()
{
    TEST_ASSERT_EQUAL(0, list_remove(NULL, (void *)0xdeadbeef));
}