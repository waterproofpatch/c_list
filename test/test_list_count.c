#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

// code under test
#include "list.h"

void setUp()
{
}

void tearDown()
{
}

void test_list_count_success()
{
    list_t list = {.count = 1};
    TEST_ASSERT_EQUAL(1, list_count(&list));
}

void test_list_count_fail_invalid_list()
{
    TEST_ASSERT_EQUAL(0, list_count(NULL));
}