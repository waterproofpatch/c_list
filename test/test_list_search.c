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
 * @brief verify we're called once and find the element
 *
 * @param element to compare
 * @param key to use for comparison
 * @return char 1 to indicate we matched the item
 */
char stub_test_list_search_success_key_comparator(void *element, void *key)
{
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, element);
    TEST_ASSERT_EQUAL((void *)0xdeadbeef, key);
    return 1;
}

void test_list_search_success()
{
    /* Locals */
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = NULL};
    list_t      list    = {.count = 1, .head = &entry_1, .tail = &entry_1};

    /* Invoke code under test */
    TEST_ASSERT_EQUAL(entry_1.element,
                      list_search(&list,
                                  stub_test_list_search_success_key_comparator,
                                  (void *)0xdeadbeef));
}

/**
 * @brief verify that we find the element after the second call
 *
 * @param element the element to test
 * @param key the key used to compare
 * @return char 0 on first call, 1 on second call
 */
char stub_test_list_search_success_two_elements_key_comparator(void *element,
                                                               void *key)
{
    static int num_calls = 0;
    if (num_calls == 0)
    {
        TEST_ASSERT_EQUAL((void *)0xdeadbeef, element);
        TEST_ASSERT_EQUAL((void *)0xfeedface, key);
    }
    else
    {
        TEST_ASSERT_EQUAL(1, num_calls);
        TEST_ASSERT_EQUAL((void *)0xfeedface, element);
        TEST_ASSERT_EQUAL((void *)0xfeedface, key);
        return 1;
    }
    num_calls++;
    return 0;
}

void test_list_search_two_elements_success()
{
    /* Locals */
    list_node_t entry_2 = {.element = (void *)0xfeedface, .next = NULL};
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = &entry_2};
    list_t      list    = {.count = 2, .head = &entry_1, .tail = &entry_2};

    /* Invoke code under test */
    TEST_ASSERT_EQUAL(
        entry_2.element,
        list_search(&list,
                    stub_test_list_search_success_two_elements_key_comparator,
                    (void *)0xfeedface));
}

/**
 * @brief verify we're called correctly and don't find the element
 *
 * @param element to test
 * @param key to use to compare
 * @return char 0 for not finding the element
 */
char stub_test_list_search_not_found_key_comparator(void *element, void *key)
{
    static int num_calls            = 0;
    void *     expected_elements[2] = {(void *)0xdeadbeef, (void *)0xfeedface};
    TEST_ASSERT_EQUAL(0xf00dface, key);
    TEST_ASSERT_EQUAL(expected_elements[num_calls], element);
    num_calls++;
    return 0;
}

void test_list_search_not_found()
{
    /* Locals */
    list_node_t entry_2 = {.element = (void *)0xfeedface, .next = NULL};
    list_node_t entry_1 = {.element = (void *)0xdeadbeef, .next = &entry_2};
    list_t      list    = {.count = 2, .head = &entry_1, .tail = &entry_2};

    /* Invoke code under test */
    TEST_ASSERT_EQUAL(
        NULL,
        list_search(&list,
                    stub_test_list_search_not_found_key_comparator,
                    (void *)0xf00dface));
}

void test_list_search_invalid_list()
{
    /* Invoke code under test */
    TEST_ASSERT_EQUAL(
        NULL,
        list_search(
            NULL, (char (*)(void *, void *))0xdeadbeef, (void *)0xf00dface));
}

void test_list_search_invalid_key_comparator()
{
    /* Invoke code under test */
    TEST_ASSERT_EQUAL(NULL,
                      list_search((void *)0xdeadbeef,
                                  (char (*)(void *, void *))NULL,
                                  (void *)0xf00dface));
}

void test_list_search_invalid_key_key()
{
    /* Invoke code under test */
    TEST_ASSERT_EQUAL(NULL,
                      list_search((void *)0xdeadbeef,
                                  (char (*)(void *, void *))0xfeedface,
                                  (void *)NULL));
}