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
 * @brief Create a new list.
 * @param list_malloc The allocator to use for creating list nodes.
 * @param list_free The free function to use for desrtoying list nodes.
 * @return A ptr to a list_t object.
 * @return NULL if no resources could be allocated.
 */
list_t *list_init(void *(*list_malloc)(size_t), void (*list_free)(void *));

/**
 * @brief Destroy a list.
 * @param list The list to destroy.
 */
void list_destroy(list_t *list);

/**
 * @brief Add an element to the list (shallow copy).
 * @param list The list to add to.
 * @param element The element to add to the list.
 * @return 1 if adding element was successful.
 * @return 0 if adding element failed..
 */
int list_add(list_t *list, void *element);

/**
 * @brief Remove an element from the list.
 * @param list The list from which removal should occur.
 * @param element The element to remove from the list.
 * @return 1 if removal was successful.
 * @return 0 if removal was unsuccessful.
 */
int list_remove(list_t *list, void *element);

/**
 * @brief Invoke a user-supplied function on each element in the list.
 * @param list The list to process.
 * @param process_func The function to invoke on each element of the list.
 * @param context An opaque context object to pass to the list.
 */
void list_foreach(list_t *list,
                  void (*process_func)(void *, void *),
                  void *context);

/**
 * @brief Get an element at index.
 * @param list The list to search.
 * @param index The index into the list to get.
 * @return Ptr to element in list at index 'index'.
 * @return NULL if no entry exists at index.
 */
void *list_get_at_index(list_t *list, unsigned int index);

/**
 * @brief Searches the supplied list for an element with a key matching a
 * supplied key. key_comparators should return 1 on a match, 0 otherwise. This
 * function returns the first element that caused key_comparator to return 1.
 * @param list The list to search.
 * @param key_comparator The supplied comparator function.
 * @param key The key to pass along with the element to the key_comparator
 * function.
 * @return A ptr to the matching element if any found.
 * @return NULL if key_comparator does not return '1' at all.
 */
void *list_search(list_t *list,
                  char (*key_comparator)(void *, void *),
                  void *key);

/**
 * @brief Get the count of the list.
 * @param list The list to get the count of.
 * @return Number of entries in the list.
 */
size_t list_count(list_t *list);

#endif   // LIST_H
