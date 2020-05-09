#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

/* code under test */
#include "list.h"

/* test globals */
static int g_num_frees = 0;

void setUp()
{
    g_num_frees = 0;
}

void tearDown()
{
}

/**
 * @brief verify that free is called with the correct element
 *
 * @param ptr to element to free
 */
void stub_free(void *ptr)
{
    TEST_ASSERT_NOT_NULL(ptr);
    g_num_frees++;
}

void test_list_destroy_success()
{
    /* Locals */
    list_t list = {.list_malloc = (void *(*)(size_t))0xdeadbeef,
                   .list_free   = stub_free,
                   .head        = NULL,
                   .tail        = NULL,
                   .count       = 0};

    /* Invoke function under test */
    list_destroy(&list);

    /* Assertions */
    TEST_ASSERT_EQUAL(1, g_num_frees);
    TEST_ASSERT_EQUAL(0, list.count);
}

void test_list_destroy_success_one_entry()
{
    /* Locals */
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_t      list    = {.list_malloc = (void *(*)(size_t))0xdeadbeef,
                   .list_free   = stub_free,
                   .head        = &entry_1,
                   .tail        = &entry_1,
                   .count       = 1};

    /* Invoke function under test */
    list_destroy(&list);

    /* Assertions */
    TEST_ASSERT_EQUAL(2, g_num_frees);
}

void test_list_destroy_success_two_entries()
{
    /* Locals */
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_node_t entry_2 = {.element = (void *)0xfeedface, .next = &entry_1};
    list_t      list    = {.list_malloc = (void *(*)(size_t))0xdeadbeef,
                   .list_free   = stub_free,
                   .head        = &entry_2,
                   .tail        = &entry_1,
                   .count       = 1};

    /* Invoke function under test */
    list_destroy(&list);

    /* Assertions */
    TEST_ASSERT_EQUAL(3, g_num_frees);
}

void test_list_destroy_fail_invalid_list()
{
    /* Locals */
    /* Invoke function under test */
    list_destroy(NULL);

    /* Assertions */
    TEST_ASSERT_EQUAL(0, g_num_frees);
}
