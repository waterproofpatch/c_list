#include <stddef.h>
#include <stdlib.h>
#include "unity.h"

// mocks
#include "mock_std_lib_wrapper.h"

// code under test
#include "list.h"

int g_allocs = 0;
int g_frees  = 0;

void setUp()
{
    g_allocs = 0;
    g_frees  = 0;
}

void tearDown()
{
    TEST_ASSERT_EQUAL(0, g_allocs - g_frees);
}

void proc_func_assert_null(void *element, void *context)
{
    TEST_ASSERT_NULL(context);
}

void proc_func(void *element, void *context)
{
    static int num_proc_calls;
    num_proc_calls++;
    TEST_ASSERT_EQUAL(101, *(int *)context);
    TEST_ASSERT_EQUAL(num_proc_calls, *(int *)element);
}

char comparator(void *e1, void *e2)
{
    // neither of these should be NULL
    TEST_ASSERT_NOT_NULL(e1);
    TEST_ASSERT_NOT_NULL(e2);

    // we know the type is int
    int e1_val = *(int *)e1;
    int e2_val = *(int *)e2;

    // return the diff
    return (e2_val - e1_val) == 0;
}

void *wrap_malloc_stub(size_t size, int count)
{
    g_allocs++;
    return malloc(size);
}

void wrap_free_stub(void *ptr, int count)
{
    g_frees++;
    return free(ptr);
}

void testListInitNoMem()
{
    wrap_malloc_ExpectAndReturn(sizeof(list_t), NULL);
    list_t *list = list_init(wrap_malloc, wrap_free);
    TEST_ASSERT_NULL(list);
}

void testListInit()
{
    list_t *ret_list = malloc(sizeof(list_t));
    wrap_malloc_ExpectAndReturn(sizeof(list_t), ret_list);
    wrap_free_Expect(ret_list);

    list_t *list = list_init(wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(list);

    free(ret_list);
    list_destroy(list);
}

void testListInitNullMalloc()
{
    TEST_ASSERT_NULL(list_init(NULL, wrap_free));
}

void testListInitNullFree()
{
    TEST_ASSERT_NULL(list_init(wrap_malloc, NULL));
}

void testListCount()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);

    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    int     elem_2 = 2;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_count(list));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    TEST_ASSERT_EQUAL(2, list_count(list));
    TEST_ASSERT_EQUAL(1, list_remove(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_count(list));
    list_destroy(list);
}

void testListAddRemoveAdd()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);

    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_remove(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    void *res = list_get_at_index(list, 0);
    TEST_ASSERT_EQUAL(&elem_1, res);
    TEST_ASSERT_EQUAL(1, list_remove(list, &elem_1));
    res = list_get_at_index(list, 0);
    TEST_ASSERT_EQUAL(NULL, res);
    list_destroy(list);
}

void testListForEach()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);

    list_t *list    = list_init(wrap_malloc, wrap_free);
    int     elem_1  = 1;
    int     elem_2  = 2;
    int     elem_3  = 3;
    int     elem_4  = 4;
    int     context = 101;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_3));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_4));
    list_foreach(list, proc_func, &context);
    list_destroy(list);
}

void testListForEachNullContext()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);

    list_t *list    = list_init(wrap_malloc, wrap_free);
    int     elem_1  = 1;
    int     elem_2  = 2;
    int     elem_3  = 3;
    int     elem_4  = 4;
    int     context = 101;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_3));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_4));
    list_foreach(list, proc_func_assert_null, NULL);
    list_destroy(list);
}

void testListAddAndRemoveSingleElement()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);

    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list->count);
    TEST_ASSERT_EQUAL(list->head, list->tail);
    TEST_ASSERT_EQUAL(1, list_remove(list, &elem_1));
    TEST_ASSERT_EQUAL(0, list->count);
    TEST_ASSERT_EQUAL(NULL, list->head);
    TEST_ASSERT_EQUAL(NULL, list->tail);
    list_destroy(list);
}

void testListRemoveElemNotExist()
{
    wrap_free_StubWithCallback(wrap_free_stub);
    wrap_malloc_StubWithCallback(wrap_malloc_stub);

    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    int     elem_2 = 2;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(0, list_remove(list, &elem_2));
    list_destroy(list);
}
void testListRemoveSecondElement()
{
    wrap_free_StubWithCallback(wrap_free_stub);
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    int     elem_2 = 2;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    TEST_ASSERT_EQUAL(1, list_remove(list, &elem_2));
    list_destroy(list);
}

void testListAddNoMemory()
{
    list_t *list_alloc = (list_t *)malloc(sizeof(list_t));
    g_allocs++;   // since we're making this allocation outside the wrapper
    wrap_free_StubWithCallback(wrap_free_stub);
    wrap_malloc_ExpectAndReturn(sizeof(list_t), list_alloc);
    wrap_malloc_ExpectAndReturn(sizeof(list_node_t), NULL);
    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    TEST_ASSERT_EQUAL(0, list_add(list, &elem_1));
    list_destroy(list);
}

void testListAddTwoElementsAndDestroy()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);
    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    int     elem_2 = 2;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    TEST_ASSERT_EQUAL(2, list->count);
    list_destroy(list);
}

void testListGetAtIndex()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);
    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    int     elem_2 = 2;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    int elem_at_index_1 = *(int *)list_get_at_index(list, 0);
    int elem_at_index_2 = *(int *)list_get_at_index(list, 1);
    TEST_ASSERT_EQUAL(1, elem_at_index_1);
    TEST_ASSERT_EQUAL(2, elem_at_index_2);
    TEST_ASSERT_EQUAL(NULL, list_get_at_index(list, 2));
    list_destroy(list);
}

void testListSearchNoElements()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);
    list_t *list = list_init(wrap_malloc, wrap_free);
    int     key  = 1;
    void *  res  = list_search(list, comparator, &key);
    TEST_ASSERT_NULL(res);
    list_destroy(list);
}

void testListSearchOneElement()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);
    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    int   key = 1;
    void *res = list_search(list, comparator, &key);
    TEST_ASSERT_NOT_NULL(res);
    TEST_ASSERT_EQUAL(1, *(int *)res);
    list_destroy(list);
}

void testListSearchThreeElements()
{
    wrap_malloc_StubWithCallback(wrap_malloc_stub);
    wrap_free_StubWithCallback(wrap_free_stub);
    list_t *list   = list_init(wrap_malloc, wrap_free);
    int     elem_1 = 1;
    int     elem_2 = 2;
    int     elem_3 = 3;
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_1));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_2));
    TEST_ASSERT_EQUAL(1, list_add(list, &elem_3));

    int   key_1 = 1;
    int   key_2 = 2;
    int   key_3 = 3;
    void *res_1 = list_search(list, comparator, &key_1);
    void *res_2 = list_search(list, comparator, &key_2);
    void *res_3 = list_search(list, comparator, &key_3);
    TEST_ASSERT_NOT_NULL(res_1);
    TEST_ASSERT_EQUAL(1, *(int *)res_1);
    TEST_ASSERT_NOT_NULL(res_2);
    TEST_ASSERT_EQUAL(2, *(int *)res_2);
    TEST_ASSERT_NOT_NULL(res_3);
    TEST_ASSERT_EQUAL(3, *(int *)res_3);

    list_destroy(list);
}
