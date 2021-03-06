#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

/* code under test */
#include "list.h"

/* test globals */
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

static void stub_list_free(void *ptr)
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
    /* Locals */
    list_node_t node_2 = {.next = NULL, .element = (void *)2};
    list_node_t node_1 = {.next = &node_2, .element = (void *)1};
    list_t      list   = {.head      = &node_1,
                   .tail      = &node_2,
                   .count     = 2,
                   .list_free = stub_list_free};

    g_expected = &node_2;

    /* Invoke code under test */
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
    /* Locals */
    list_node_t node_2 = {.next = NULL, .element = (void *)2};
    list_node_t node_1 = {.next = &node_2, .element = (void *)1};
    list_t      list   = {.head      = &node_1,
                   .tail      = &node_2,
                   .count     = 2,
                   .list_free = stub_list_free};

    g_expected = &node_1;

    /* Invoke code under test */
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
    /* Locals */
    list_node_t node_3 = {.next = NULL, .element = (void *)3};
    list_node_t node_2 = {.next = &node_3, .element = (void *)2};
    list_node_t node_1 = {.next = &node_2, .element = (void *)1};
    list_t      list   = {.head      = &node_1,
                   .tail      = &node_3,
                   .count     = 3,
                   .list_free = stub_list_free};

    g_expected = &node_2; /* the middle */

    /* Invoke code under test */
    TEST_ASSERT_EQUAL(1, list_remove(&list, node_2.element));
    TEST_ASSERT_EQUAL(2, list.count);
    TEST_ASSERT_EQUAL(&node_1, list.head);
    TEST_ASSERT_EQUAL(&node_3, list.tail);
    TEST_ASSERT_EQUAL(1, g_num_frees);
}

void test_list_remove_does_not_exist()
{
    /* Locals */
    list_node_t node_3 = {.next = NULL, .element = (void *)3};
    list_node_t node_2 = {.next = NULL, .element = (void *)2};
    list_node_t node_1 = {.next = &node_3, .element = (void *)1};
    list_t      list   = {.head      = &node_1,
                   .tail      = &node_3,
                   .count     = 2,
                   .list_free = stub_list_free};

    /* Invoke code under test */
    TEST_ASSERT_EQUAL(0, list_remove(&list, node_2.element));

    /* Assertions */
    TEST_ASSERT_EQUAL(2, list.count);
    TEST_ASSERT_EQUAL(&node_1, list.head);
    TEST_ASSERT_EQUAL(&node_3, list.tail);
    TEST_ASSERT_EQUAL(0, g_num_frees);
}

void test_list_remove_fail_invalid_element()
{
    /* Invoke code under test */
    TEST_ASSERT_EQUAL(0, list_remove((list_t *)0xdeadbeef, NULL));
}

void test_list_remove_fail_invalid_list()
{
    /* Invoke code under test */
    TEST_ASSERT_EQUAL(0, list_remove(NULL, (void *)0xdeadbeef));
}