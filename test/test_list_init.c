#include <stddef.h>
#include <stdlib.h>
#include <string.h> /* memset */

#include "unity.h"

/* code under test */
#include "list.h"

static list_t g_list       = {0};
static int    g_num_allocs = 0;

void setUp()
{
    memset(&g_list, 0, sizeof(list_t));
    g_num_allocs = 0;
}

void tearDown()
{
}

/**
 * @brief verify that we allocate resources for the list structure
 *
 * @param size of the list structure
 * @return void* newly created resource
 */
static void *stub_test_list_init_success_list_malloc(size_t size)
{
    TEST_ASSERT_EQUAL(sizeof(list_t), size);
    g_num_allocs++;
    return &g_list;
}

void test_list_init_success()
{
    /* Invoke code under test */
    list_t *list = list_init(stub_test_list_init_success_list_malloc,
                             (void (*)(void *))0xdeadbeef);

    /* Assertions */
    TEST_ASSERT_EQUAL(&g_list, list);
    TEST_ASSERT_EQUAL(stub_test_list_init_success_list_malloc,
                      list->list_malloc);
    TEST_ASSERT_EQUAL(0xdeadbeef, list->list_free);
    TEST_ASSERT_EQUAL(NULL, list->head);
    TEST_ASSERT_EQUAL(NULL, list->tail);
    TEST_ASSERT_EQUAL(0, list->count);
}

void test_list_init_fail_invalid_list_malloc()
{
    /* Invoke code under test */
    list_t *list = list_init(NULL, (void (*)(void *))0xdeadbeef);

    /* Assertions */
    TEST_ASSERT_EQUAL(NULL, list);
}

void test_list_init_fail_invalid_list_free()
{
    /* Invoke code under test */
    list_t *list = list_init(stub_test_list_init_success_list_malloc, NULL);

    /* Assertions */
    TEST_ASSERT_EQUAL(NULL, list);
}

static void *stub_test_list_init_fail_no_mem_list_malloc(size_t size)
{
    TEST_ASSERT_EQUAL(sizeof(list_t), size);
    return NULL;
}
void test_list_init_fail_no_mem()
{
    /* Invoke code under test */
    list_t *list = list_init(stub_test_list_init_fail_no_mem_list_malloc,
                             (void (*)(void *))0xdeadbeef);

    /* Assertions */
    TEST_ASSERT_EQUAL(NULL, list);
}