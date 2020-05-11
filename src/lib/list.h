/**
 *
 * @author waterproofpatch
 *
 * Public interface for the linked list implementation.
 */
#ifndef LIST_H
#define LIST_H

#include <stddef.h>

/**
 * @brief A single data entry within the list.
 */
typedef struct _list_node_t
{
    struct _list_node_t *next; /**< pointer to next element in the list */
    void *element; /**< pointer to the value element, allocated by user */
} list_node_t;

/**
 * @brief List structure. Contains ptr to head and tail and
 * addresses of functions to use for resource allocation and release.
 *
 */
typedef struct _list_t
{
    list_node_t *head;            /**< front of the list */
    list_node_t *tail;            /**< back of the list */
    void *(*list_malloc)(size_t); /**< routine for creating new resources */
    void (*list_free)(void *);    /**< routine for releasing resources */
    size_t count;                 /**< number of elements in the list */
} list_t;

/**
 * @brief create a list object
 * @param list_malloc: the allocator to use for creating list nodes
 * @param list_free: the free function to use for desrtoying list nodes
 * @return a ptr to a list_t object
 */
list_t *list_init(void *(*list_malloc)(size_t), void (*list_free)(void *));

/**
 * @brief Destroy a list.
 * @param list: the list to destroy
 * @return: void
 */
void list_destroy(list_t *list);

/**
 * @brief add an element to the list (shallow copy)
 * @param list: the list to add to
 * @param element: the element to add to the list
 * @return: 1 if adding element was successful, 0 otherwise
 */
int list_add(list_t *list, void *element);

/**
 * @brief removal based on linux torvold's linked list impl. Does NOT call
 * 'list_free' on elements. elements
 * @param list: the list from which removal should occur
 * @param element: the element to remove from the list
 */
int list_remove(list_t *list, void *element);

/**
 * @brief invoke a function on each element in the list
 * @param list: the list to process
 * @param process_func: the function to invoke on each element of the list
 * @param context: an opaquie context object to pass to the list
 * @return: void
 */
void list_foreach(list_t *list,
                  void (*process_func)(void *, void *),
                  void *context);

/**
 * @brief get an element at index
 * @param list: the list to search
 * @param index: the index into the list to get
 * @return ptr to element in list at index 'index', NULL if no data found at
 * index
 */
void *list_get_at_index(list_t *list, unsigned int index);

/**
 * @brief searches the supplied list for an element with a key matching a
 * supplied key. key_comparators should return 1 on a match, 0 otherwise. This
 * function returns the first element that caused key_comparator to return 1.
 * @param list: the list to search
 * @param key_comparator: the supplied comparator function
 * @param key: the key to pass along with the element to the key_comparator
 * function
 * @return: a ptr to the matching element if any found, NULL otherwise
 */
void *list_search(list_t *list,
                  char (*key_comparator)(void *, void *),
                  void *key);

/**
 * @brief get the count of the list
 * @param list: the list to get the count of
 * @return: number of entries in the list
 */
size_t list_count(list_t *list);

#endif   // LIST_H
