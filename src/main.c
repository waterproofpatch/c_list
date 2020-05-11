/**
 * @author waterproofpatch
 *
 * Test driver
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* lib includes */
#include "list.h"

/**
 * @brief Some example struct
 *
 */
typedef struct _element_t
{
    unsigned int   a; /** dummy entry */
    unsigned char  b; /** dummy entry */
    unsigned short c; /** dummy entry */
} element_t;

/**
 * @brief some generic processing routine
 *
 * @param item to process
 * @param context user supplied argument
 */
static void item_processor(void *item, void *context)
{
    printf("got item: '%p', context '%p'\n", item, context);
}

/**
 * @brief Create a new element object
 *
 * @param a some dummy value
 * @param b some dummy value
 * @param c some dummy value
 * @return element_t*
 */
static element_t *create_new_element(unsigned int   a,
                                     unsigned int   b,
                                     unsigned short c)
{
    element_t *elem = (element_t *)malloc(sizeof(element_t *));
    if (elem == NULL)
    {
        printf("Failed allocating a new element\n");
        return NULL;
    }
    elem->a = a;
    elem->b = b;
    elem->c = c;
    return elem;
}

/**
 * @brief print each element in the list
 *
 * @param element data entry
 * @param context user-supplied context (argument(s))
 */
static void print_list(void *element, void *context)
{
    if (element == NULL)
    {
        printf("invalid argument\n");
        return;
    }
    element_t *elem = (element_t *)element;
    printf("elem->a: 0x%08x\nelem->b: %c\nelem->c: 0x%04x\n",
           elem->a,
           elem->b,
           elem->c);
}

/**
 * @brief compare like-elements in this list.
 *
 * @param element
 * @param context user-supplied context (argument(s))
 * @return char 1 if the two elements are equal, 0 otherwise
 */
static char element_comparator(void *element, void *context)
{
    /* need both these things to be non null */
    if (element == NULL || context == NULL)
    {
        return 0;
    }

    element_t *elem = (element_t *)element;
    char       key  = *(char *)context;

    if (elem->b == key)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief Entry point.
 *
 * @param argc unused
 * @param argv unused
 * @return int
 */
int main(int argc, char **argv)
{
    // item to add to list
    char *mystring1 = (char *)malloc(strlen("this is my string1") + 1);
    char *mystring2 = (char *)malloc(strlen("this is my string2") + 1);
    strcpy(mystring1, "this is my string1");
    strcpy(mystring2, "this is my string2");

    // create a list
    list_t *list = list_init(malloc, free);
    assert(list != NULL);

    // add our item
    assert(list_add(list, mystring1));
    assert(list_add(list, mystring2));

    // process each element
    list_foreach(list, item_processor, "pass me");

    // destroy the list
    list_destroy(list);

    // re-initialize the list
    list = list_init(malloc, free);
    assert(list != NULL);

    element_t *a = create_new_element(0xdeadbeef, 'a', 0xdead);
    element_t *b = create_new_element(0xdeadbeef, 'b', 0xdead);
    element_t *c = create_new_element(0xdeadbeef, 'c', 0xdead);
    element_t *d = create_new_element(0xdeadbeef, 'd', 0xdead);
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);
    assert(d != NULL);

    assert(list != NULL);

    assert(list_count(list) == 0);
    assert(list_add(list, a));
    assert(list_add(list, b));
    assert(list_add(list, c));
    assert(list_add(list, d));
    assert(list_count(list) == 4);

    element_t *elem0 = list_get_at_index(list, 2);
    assert(elem0 == c);
    assert(elem0->a == 0xdeadbeef);
    assert(elem0->b == 'c');

    element_t *elem2 = list_get_at_index(list, 8);
    assert(elem2 == NULL);

    char       key0       = 'a';
    element_t *matching_a = list_search(list, element_comparator, &key0);
    assert(matching_a == a);
    assert(matching_a->b == key0);

    char       key1       = 'd';
    element_t *matching_d = list_search(list, element_comparator, &key1);
    assert(matching_d == d);
    assert(matching_d->b == key1);

    char       key2       = 'h';
    element_t *matching_h = list_search(list, element_comparator, &key2);
    assert(matching_h == NULL);

    list_foreach(list, print_list, NULL);

    assert(list_remove(list, c));
    free(c);
    assert(list_count(list) == 3);

    list_foreach(list, print_list, NULL);

    assert(list_remove(list, a));
    free(a);
    assert(list_count(list) == 2);

    list_foreach(list, print_list, NULL);

    assert(list_remove(list, d));
    free(d);
    assert(list_count(list) == 1);

    list_foreach(list, print_list, NULL);

    list_destroy(list);

    return 0;
}
