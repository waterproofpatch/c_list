/**
 * @author waterproofpatch
 * @brief Test driver
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* sample data includes */
#include "example1.h"

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

/* prototypes, defined below */
static void       print_list(void *element, void *context);
static char       element_comparator(void *element, void *context);
static element_t *create_new_element(unsigned int   a,
                                     unsigned int   b,
                                     unsigned short c);

/**
 * @brief Entry point.
 *
 * @param argc unused
 * @param argv unused
 * @return int
 */
int main(int argc, char **argv)
{
    // kick off the example, showing some usage
    example1_run();

    element_t *a = create_new_element(0xdeadbeef, 'a', 0xdead);
    element_t *b = create_new_element(0xdeadbeef, 'b', 0xdead);
    element_t *c = create_new_element(0xdeadbeef, 'c', 0xdead);
    element_t *d = create_new_element(0xdeadbeef, 'd', 0xdead);

    printf("Creating list and adding elements...\n");
    list_t *list = list_init(malloc, free);

    printf("Printing list\n");
    list_foreach(list, print_list, NULL);

    printf("Destroying list...\n");
    list_destroy(list);

    printf("Creating list and adding elements...\n");
    list = list_init(malloc, free);

    assert(list_count(list) == 0);
    list_add(list, a);
    list_add(list, b);
    list_add(list, c);
    list_add(list, d);
    assert(list_count(list) == 4);

    printf("Getting element at index 2\n");
    element_t *elem0 = list_get_at_index(list, 2);
    assert(elem0 != NULL);
    assert(elem0->a == 0xdeadbeef);
    assert(elem0->b == 'c');

    printf("Getting element at index 8\n");
    element_t *elem2 = list_get_at_index(list, 8);
    assert(elem2 == NULL);

    printf("Searching for 'a'\n");
    char       key0       = 'a';
    element_t *matching_a = list_search(list, element_comparator, &key0);
    assert(matching_a != NULL);
    assert(matching_a->b == key0);

    printf("Searching for 'd'\n");
    char       key1       = 'd';
    element_t *matching_d = list_search(list, element_comparator, &key1);
    assert(matching_d != NULL);
    assert(matching_d->b == key1);

    printf("Searching for 'h'\n");
    char       key2       = 'h';
    element_t *matching_h = list_search(list, element_comparator, &key2);
    assert(matching_h == NULL);

    printf("Printing list\n");
    list_foreach(list, print_list, NULL);

    printf("Removing %c\n", c->b);
    list_remove(list, c);
    free(c);
    assert(list_count(list) == 3);

    printf("Printing list\n");
    list_foreach(list, print_list, NULL);

    printf("Removing %c\n", a->b);
    list_remove(list, a);
    free(a);
    assert(list_count(list) == 2);

    printf("Printing list\n");
    list_foreach(list, print_list, NULL);

    printf("Removing %c\n", d->b);
    list_remove(list, d);
    free(d);
    assert(list_count(list) == 1);

    printf("Printing list\n");
    list_foreach(list, print_list, NULL);

    printf("Destroying list...\n");
    list_destroy(list);
}

/**
 * @brief Create a new element object
 *
 * @param a some dummy value
 * @param b some dummy value
 * @param c some dummy value
 * @return element_t*
 */
element_t *create_new_element(unsigned int a, unsigned int b, unsigned short c)
{
    element_t *elem = (element_t *)malloc(sizeof(element_t *));
    elem->a         = a;
    elem->b         = b;
    elem->c         = c;
    return elem;
}

/**
 * @brief print each element in the list
 *
 * @param element data entry
 * @param context user-supplied context (argument(s))
 */
void print_list(void *element, void *context)
{
    element_t *elem = (element_t *)element;
    printf("elem->a: 0x%08x\nelem->b: %c\nelem->c: 0x%04x\n", elem->a, elem->b,
           elem->c);
}

/**
 * @brief compare like-elements in this list.
 *
 * @param element
 * @param context user-supplied context (argument(s))
 * @return char 1 if the two elements are equal, 0 otherwise
 */
char element_comparator(void *element, void *context)
{
    element_t *elem = (element_t *)element;
    char       key  = *(char *)context;
    if (elem->b == key)
    {
        return 1;
    }
    return 0;
}
