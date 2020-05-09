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

void stub_test_list_foreach_success_null_context_process_func(void *elem,
                                                              void *context)
{
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, elem);
    TEST_ASSERT_NULL(context);
}

void test_list_foreach_success_null_context()
{
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};

    list_t list = {.head        = &entry_1,
                   .tail        = &entry_1,
                   .count       = 0,
                   .list_free   = stub_free,
                   .list_malloc = stub_malloc};

    list_foreach(
        &list, stub_test_list_foreach_success_null_context_process_func, NULL);
}

void stub_test_list_foreach_success_non_null_context_process_func(void *elem,
                                                                  void *context)
{
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_NOT_NULL(context);
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, elem);
    TEST_ASSERT_EQUAL((void *)0xfeedface, context);
}

void test_list_foreach_success_non_null_context()
{
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};

    list_t list = {.head        = &entry_1,
                   .tail        = &entry_1,
                   .count       = 0,
                   .list_free   = stub_free,
                   .list_malloc = stub_malloc};

    list_foreach(&list,
                 stub_test_list_foreach_success_non_null_context_process_func,
                 (void *)0xfeedface);
}

void test_list_foreach_invalid_list()
{
    list_foreach(NULL, (void *)0xdeadf00d, (void *)0xfeedface);
}

void test_list_foreach_invalid_process_func()
{
    list_foreach((void *)0xdeadbeef, NULL, (void *)0xfeedface);
}