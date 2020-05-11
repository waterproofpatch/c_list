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
 * @brief verify process_func is called correctly
 *
 * @param elem element to process
 * @param context optional argument to process (should be NULL)
 */
static void stub_test_list_foreach_success_null_context_process_func(
    void *elem,
    void *context)
{
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, elem);
    TEST_ASSERT_NULL(context);
}

void test_list_foreach_success_null_context()
{
    /* Locals */
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_t      list    = {.head        = &entry_1,
                   .tail        = &entry_1,
                   .count       = 0,
                   .list_free   = (void (*)(void *))0xdeadbeef,
                   .list_malloc = (void *(*)(size_t))0xfeedface};

    /* Inoke code under test*/
    list_foreach(
        &list, stub_test_list_foreach_success_null_context_process_func, NULL);
}

/**
 * @brief verify process-func is called correctly with a non-null context
 *
 * @param elem element to process
 * @param context optional argument to process (should not be NULL)
 */
static void stub_test_list_foreach_success_non_null_context_process_func(
    void *elem,
    void *context)
{
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_NOT_NULL(context);
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, elem);
    TEST_ASSERT_EQUAL((void *)0xfeedface, context);
}

void test_list_foreach_success_non_null_context()
{
    /* Locals */
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};

    list_t list = {.head        = &entry_1,
                   .tail        = &entry_1,
                   .count       = 0,
                   .list_free   = (void (*)(void *))0xdeadbeef,
                   .list_malloc = (void *(*)(size_t))0xf00dface};

    /* Inoke code under test*/
    list_foreach(&list,
                 stub_test_list_foreach_success_non_null_context_process_func,
                 (void *)0xfeedface);
}

void test_list_foreach_invalid_list()
{
    /* Inoke code under test*/
    list_foreach(NULL, (void *)0xdeadf00d, (void *)0xfeedface);
}

void test_list_foreach_invalid_process_func()
{
    /* Inoke code under test*/
    list_foreach((void *)0xdeadbeef, NULL, (void *)0xfeedface);
}