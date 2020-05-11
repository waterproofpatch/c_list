/**
 * @author waterproofpatch
 * @file list.c
 * @brief List implementation.
 */
#include <stdlib.h>

/* Foreard declarations for this file */
#include "list.h"

list_t *list_init(void *(*list_malloc)(size_t), void (*list_free)(void *))
{
    /* Validate args */
    if (list_malloc == NULL || list_free == NULL)
    {
        return NULL;
    }

    /* Create a new list structure and initialize its members */
    list_t *list = (list_t *)list_malloc(sizeof(list_t));
    if (list == NULL)
    {
        return NULL;
    }
    list->head = list->tail = NULL;
    list->count             = 0;
    list->list_malloc       = list_malloc;
    list->list_free         = list_free;
    return list;
}

void list_destroy(list_t *list)
{
    /* Validate args */
    if (list == NULL)
    {
        return;
    }

    /* Release resources for each list_node_t */
    list_node_t *cur  = list->head;
    list_node_t *next = NULL;
    while (cur)
    {
        next = cur->next;
        list->list_free(cur);
        cur = next;
    }
    /* .. and then finally for the list itself */
    list->list_free(list);
}

int list_add(list_t *list, void *element)
{
    /* Validate args */
    if (list == NULL || element == NULL)
    {
        return 0;
    }

    /* Create a new entry to hold a reference to the data */
    list_node_t *new_node =
        (list_node_t *)list->list_malloc(sizeof(list_node_t));
    if (new_node == NULL)
    {
        return 0;
    }
    new_node->next    = NULL;
    new_node->element = element;

    /* Case where new element is the first element */
    if (list->head == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
        list->count++;
        return 1;
    }

    /* Case where new element is not the first element */
    list->tail->next = new_node;
    list->tail       = new_node;
    list->count++;
    return 1;
}

int list_remove(list_t *list, void *element)
{
    /* Validate args */
    if (list == NULL || element == NULL)
    {
        return 0;
    }

    list_node_t *cur  = list->head;
    list_node_t *prev = NULL;

    /* Compare pointers until we find a matching entry */
    while (cur && cur->element != element)
    {
        prev = cur;
        cur  = cur->next;
    }

    /* Not found */
    if (!cur)
    {
        return 0;
    }

    /* If we're removing the tail, move the tail back */
    if (cur == list->tail)
    {
        list->tail = prev;
    }

    /* Skip entry about to be removed */
    if (prev)
    {
        prev->next = cur->next;
    }
    /* Case where head is removed */
    else
    {
        list->head = cur->next;
    }
    list->list_free(cur);
    list->count--;
    return 1;
}

void list_foreach(list_t *list,
                  void (*process_func)(void *, void *),
                  void *context)
{
    /* Validate args */
    if (list == NULL || process_func == NULL)
    {
        return;
    }

    /* Starting with the head, move through each element
     * in the list invoking the process func on it */
    list_node_t *cur = list->head;
    while (cur)
    {
        process_func(cur->element, context);
        cur = cur->next;
    }
}

void *list_get_at_index(list_t *list, unsigned int index)
{
    /* Validate args */
    if (list == NULL)
    {
        return NULL;
    }

    /* Move through the list until the requested index is found or we are out of
     * elements */
    unsigned int i   = 0;
    list_node_t *cur = list->head;
    while (cur && i++ < index)
    {
        cur = cur->next;
    }

    return cur != NULL ? cur->element : NULL;
}

void *list_search(list_t *list,
                  char (*key_comparator)(void *, void *),
                  void *key)
{
    /* Validate args */
    if (list == NULL || key_comparator == NULL || key == NULL)
    {
        return NULL;
    }

    /* Starting with the head, move through each element in the list
     * and invoke the key_comparator on it with the supplied key
     * until we have a match or we exhaust the list. */
    list_node_t *cur = list->head;
    while (cur)
    {
        if (key_comparator(cur->element, key) == 1)
        {
            return cur->element;
        }
        cur = cur->next;
    }
    return NULL;
}

inline size_t list_count(list_t *list)
{
    return list == NULL ? 0 : list->count;
}
