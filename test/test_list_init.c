#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

// code under test
#include "list.h"

static list_t g_list        = {0};
static int    g_num_allocs  = 0;
static int    g_num_frees   = 0;
static char   g_return_null = 0;

void setUp()
{
    g_return_null = 0;
}

void tearDown()
{
}

void *stub_list_malloc(size_t size)
{
    TEST_ASSERT_EQUAL(sizeof(list_t), size);
    g_num_allocs++;
    return g_return_null ? NULL : &g_list;
}

void stub_list_free(void *obj)
{
    g_num_frees++;
}

void test_list_init_success()
{
    list_t *list = list_init(stub_list_malloc, stub_list_free);
    TEST_ASSERT_EQUAL(&g_list, list);
    TEST_ASSERT_EQUAL(stub_list_malloc, list->list_malloc);
    TEST_ASSERT_EQUAL(stub_list_free, list->list_free);
    TEST_ASSERT_EQUAL(NULL, list->head);
    TEST_ASSERT_EQUAL(NULL, list->tail);
    TEST_ASSERT_EQUAL(0, list->count);
}

void test_list_init_fail_invalid_list_malloc()
{
    list_t *list = list_init(NULL, stub_list_free);
    TEST_ASSERT_EQUAL(NULL, list);
}

void test_list_init_fail_invalid_list_free()
{
    list_t *list = list_init(stub_list_malloc, NULL);
    TEST_ASSERT_EQUAL(NULL, list);
}

void test_list_init_fail_no_mem()
{
    g_return_null = 1;
    list_t *list  = list_init(stub_list_malloc, stub_list_free);
    TEST_ASSERT_EQUAL(NULL, list);
}