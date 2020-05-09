#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

// code under test
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

void stub_free(void *ptr)
{
    TEST_ASSERT_NOT_NULL(ptr);
    g_num_frees++;
}

void *stub_malloc(size_t size)
{
    /* always fail this, nothing in destroy should call malloc */
    TEST_ASSERT_FALSE_MESSAGE(1, "stub_malloc called when it should not be");
    return NULL;
}

void test_list_destroy_success()
{
    list_t list = {.list_malloc = stub_malloc,
                   .list_free   = stub_free,
                   .head        = NULL,
                   .tail        = NULL,
                   .count       = 0};
    list_destroy(&list);
    TEST_ASSERT_EQUAL(1, g_num_frees);
    TEST_ASSERT_EQUAL(0, list.count);
}

void test_list_destroy_success_one_entry()
{
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};

    list_t list = {.list_malloc = stub_malloc,
                   .list_free   = stub_free,
                   .head        = &entry_1,
                   .tail        = &entry_1,
                   .count       = 1};

    list_destroy(&list);
    TEST_ASSERT_EQUAL(2, g_num_frees);
}

void test_list_destroy_success_two_entries()
{
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_node_t entry_2 = {.element = (void *)0xfeedface, .next = &entry_1};

    list_t list = {.list_malloc = stub_malloc,
                   .list_free   = stub_free,
                   .head        = &entry_2,
                   .tail        = &entry_1,
                   .count       = 1};

    list_destroy(&list);
    TEST_ASSERT_EQUAL(3, g_num_frees);
}

void test_list_destroy_fail_invalid_list()
{
    list_destroy(NULL);
    TEST_ASSERT_EQUAL(0, g_num_frees);
}
