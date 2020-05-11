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

/**
 * @brief allocate a new list node
 *
 * @param size size of the allocation to make
 * @return void* newly created resource
 */
static void *stub_test_list_add_success_list_malloc(size_t size)
{
    TEST_ASSERT_EQUAL(sizeof(list_node_t), size);
    static list_node_t ret = {.next = NULL, .element = NULL};
    return &ret;
}

void test_list_add_success()
{
    /* Locals */
    list_t list = {.head        = NULL,
                   .tail        = NULL,
                   .count       = 0,
                   .list_malloc = stub_test_list_add_success_list_malloc};

    /* Invoke function under test */
    TEST_ASSERT_EQUAL(1, list_add(&list, (void *)0xdeadbeef));

    /* Assertions */
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, list.head->element);
    TEST_ASSERT_EQUAL(1, list.count);
}

/**
 * @brief allocate two nodes
 *
 * @param size size of the allocation to make
 * @return void* one of two potential allocations
 */
static void *stub_test_list_add_succeed_two_elements_list_malloc(size_t size)
{
    TEST_ASSERT_EQUAL(sizeof(list_node_t), size);
    static int         num_calls = 0;
    static list_node_t ret[2]    = {{.next = NULL, .element = NULL},
                                 {.next = NULL, .element = NULL}};
    num_calls++;
    return &ret[num_calls];
}

void test_list_add_success_two_elements()
{
    /* Locals */
    list_t list = {
        .head        = NULL,
        .tail        = NULL,
        .count       = 0,
        .list_malloc = stub_test_list_add_succeed_two_elements_list_malloc};

    /* Invoke function under test */
    TEST_ASSERT_EQUAL(1, list_add(&list, (void *)123));
    TEST_ASSERT_EQUAL(1, list_add(&list, (void *)345));

    /* Assertions */
    TEST_ASSERT_NOT_NULL(list.head);
    TEST_ASSERT_NOT_NULL(list.tail);
    TEST_ASSERT_EQUAL(2, list.count);
    TEST_ASSERT_EQUAL((void *)123, list.head->element);
    TEST_ASSERT_EQUAL((void *)345, list.tail->element);
}

/**
 * @brief fail to allocate
 *
 * @param size size of the allocation to make
 * @return void* NULL
 */
static void *stub_test_list_add_fail_no_mem(size_t size)
{
    TEST_ASSERT_EQUAL(sizeof(list_node_t), size);
    return NULL;
}

void test_list_add_fail_no_mem()
{
    /* Locals */
    list_t list = {.head        = NULL,
                   .tail        = NULL,
                   .count       = 0,
                   .list_malloc = stub_test_list_add_fail_no_mem};

    /* Invoke function under test */
    TEST_ASSERT_EQUAL(0, list_add(&list, (void *)123));

    /* Assertions */
    TEST_ASSERT_NULL(list.head);
    TEST_ASSERT_NULL(list.tail);
    TEST_ASSERT_EQUAL(0, list.count);
}

void test_list_add_fail_invalid_list()
{
    /* Invoke function under test */
    TEST_ASSERT_EQUAL(0, list_add(NULL, (void *)123));
}

void test_list_add_fail_invalid_element()
{
    /* Invoke function under test */
    TEST_ASSERT_EQUAL(0, list_add((list_t *)0xdeadbeef, (void *)NULL));
}