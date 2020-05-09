// #include <stddef.h>
// #include <stdlib.h>
// #include "unity.h"

// // mocks

// /* code under test */
// #include "list.h"

// static int         g_stub_malloc_calls = 0;
// static list_node_t g_test_elems[2]     = {{0}, {0}};

// void setUp()
// {
//     g_stub_malloc_calls = 0;
// }

// void tearDown()
// {
// }

// void *stub_malloc_null(size_t size)
// {
//     return NULL;
// }

// void *stub_malloc(size_t size)
// {
//     return &g_test_elems[g_stub_malloc_calls++];
// }

// void test_list_add_success()
// {
//     /* Locals */
//     list_t list      = {0};
//     int    ret       = 0;
//     list.list_malloc = stub_malloc;
//     char elem        = 'a';

//     /* Mocks */

//     /* Call function under test */
//     ret = list_add(&list, &elem);

//     /* Assertions */
//     TEST_ASSERT_EQUAL(1, ret);
//     TEST_ASSERT_EQUAL(1, list.count);
//     TEST_ASSERT_EQUAL(1, g_stub_malloc_calls);
//     TEST_ASSERT_EQUAL(&g_test_elems[0], list.head);
//     TEST_ASSERT_EQUAL(&g_test_elems[0], list.tail);
//     TEST_ASSERT_EQUAL(NULL, list.head->next);
//     TEST_ASSERT_EQUAL(NULL, list.tail->next);
// }

// void test_list_add_second_elem_success()
// {
//     /* Locals */
//     list_t list      = {0};
//     int    ret       = 0;
//     list.list_malloc = stub_malloc;
//     char elem1       = 'a';
//     char elem2       = 'b';

//     /* Mocks */

//     /* Call function under test */
//     ret = list_add(&list, &elem1);

//     /* Assertions */
//     TEST_ASSERT_EQUAL(1, ret);
//     TEST_ASSERT_EQUAL(1, list.count);
//     TEST_ASSERT_EQUAL(1, g_stub_malloc_calls);
//     TEST_ASSERT_EQUAL(&g_test_elems[0], list.head);
//     TEST_ASSERT_EQUAL(&g_test_elems[0], list.tail);
//     TEST_ASSERT_EQUAL(NULL, list.head->next);
//     TEST_ASSERT_EQUAL(NULL, list.tail->next);

//     /* Call function under test */
//     ret = list_add(&list, &elem2);

//     /* Assertions */
//     TEST_ASSERT_EQUAL(1, ret);
//     TEST_ASSERT_EQUAL(2, list.count);
//     TEST_ASSERT_EQUAL(2, g_stub_malloc_calls);
//     TEST_ASSERT_EQUAL(&g_test_elems[0], list.head);
//     TEST_ASSERT_EQUAL(&g_test_elems[1], list.head->next);
//     TEST_ASSERT_EQUAL(&g_test_elems[1], list.tail);
//     TEST_ASSERT_EQUAL(NULL, list.tail->next);
// }

// void test_list_add_fail_invalid_list()
// {
//     /* Locals */
//     int  ret   = 0;
//     char elem1 = 'a';

//     /* Mocks */

//     /* Call function under test */
//     ret = list_add(NULL, &elem1);

//     /* Assertions */
//     TEST_ASSERT_EQUAL(0, ret);
// }

// void test_list_add_fail_invalid_elem()
// {
//     /* Locals */
//     list_t list = {0};
//     int    ret  = 0;

//     /* Mocks */

//     /* Call function under test */
//     ret = list_add(&list, NULL);

//     /* Assertions */
//     TEST_ASSERT_EQUAL(0, ret);
// }

// void test_list_add_fail_list_malloc()
// {
//     /* Locals */
//     list_t list      = {0};
//     int    ret       = 0;
//     list.list_malloc = stub_malloc_null;
//     char elem        = 'a';

//     /* Mocks */

//     /* Call function under test */
//     ret = list_add(&list, &elem);

//     /* Assertions */
//     TEST_ASSERT_EQUAL(0, ret);
//     TEST_ASSERT_EQUAL(NULL, list.head);
//     TEST_ASSERT_EQUAL(NULL, list.tail);
//     TEST_ASSERT_EQUAL(0, list.count);
// }