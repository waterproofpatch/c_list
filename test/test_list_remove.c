#include <stddef.h>
#include <stdlib.h>
#include "unity.h"

// mocks
#include "mock_std_lib_wrapper.h"

// code under test
#include "list.h"

void setUp()
{
}

void tearDown()
{
}

void list_free(void* ptr)
{
    TEST_ASSERT_NOT_NULL(ptr);
    free(ptr);
}

/**
 * @brief test that we can remove the tail of the list
 * */
void test_list_remove_tail()
{
    list_t* l        = malloc(sizeof(list_t));
    l->head          = malloc(sizeof(list_node_t));
    l->head->element = (void*)4;
    l->tail          = malloc(sizeof(list_node_t));
    l->tail->element = (void*)5;
    l->tail->next    = NULL;
    l->head->next    = l->tail;
    l->count         = 2;
    l->list_free     = list_free;

    TEST_ASSERT_EQUAL(1, list_remove(l, l->tail->element));
    TEST_ASSERT_EQUAL(1, l->count);
    TEST_ASSERT_EQUAL(l->head, l->tail);
    TEST_ASSERT_EQUAL(4, l->tail->element);
    TEST_ASSERT_EQUAL(4, l->head->element);
}

/**
 * @brief test that we can remove the head of the list
 * */
void test_list_remove_head()
{
    list_t* l        = malloc(sizeof(list_t));
    l->head          = malloc(sizeof(list_node_t));
    l->head->element = (void*)4;
    l->tail          = malloc(sizeof(list_node_t));
    l->tail->element = (void*)5;
    l->tail->next    = NULL;
    l->head->next    = l->tail;
    l->count         = 2;
    l->list_free     = list_free;

    TEST_ASSERT_EQUAL(1, list_remove(l, l->head->element));
    TEST_ASSERT_EQUAL(1, l->count);
    TEST_ASSERT_EQUAL(l->head, l->tail);
    TEST_ASSERT_EQUAL(5, l->tail->element);
    TEST_ASSERT_EQUAL(5, l->head->element);
}
