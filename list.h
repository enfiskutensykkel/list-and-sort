#ifndef __LIST_H__
#define __LIST_H__

#include "common.h"
#include <stdlib.h>

/* List handle type
 * 
 * Used internally by the API.
 */
typedef struct list* list_t;



/* Callback type 
 * 
 * Used for list traversal and for freeing up list resources.
 *
 * elem - current element
 * key  - the key associated with the current element
 *
 * NB! List operations are not permitted while traversing.
 *     However, changing the element itself is permitted.
 */
typedef void (*callback_t)(point_t* elem, int key);



/* Initialize list
 *
 * Initialize an array list with an initial capacity.
 *
 * list - pointer to a list handle
 * size - initial list capacity (will grow when needed)
 *
 * Returns negative value on failure.
 */
int list_create(list_t* list, size_t size);



/* Get the number of elements in a list
 *
 * Returns the number of elements in the specified list.
 *
 * list - the list to count elements in
 *
 * Returns the number of elements in the list.
 */
size_t list_size(list_t list);



/* Traverse list
 *
 * Traverses a list and calls a user-defined callback function with 
 * each element.
 *
 * list - the list to traverse
 * cb   - the user-defined callback
 *
 * Returns a negative value on failure.
 */
int list_walk(list_t list, callback_t cb);



/* Find an element in list
 *
 * Searches for an element by key and returns the element if it is found.
 * 
 * list - the list to search from
 * key  - the key to find the element by
 * elem - if not NULL, elem will be set to the found element
 *
 * Returns 1 if element is found, 0 otherwise.
 */
int list_search(list_t list, int key, point_t** elem);



/* Insert an element into list
 *
 * Inserts an element in a list with a specified key.
 *
 * list - the list to insert into
 * key  - the key to use for the element
 * elem - the element to insert
 *
 * Returns negative value on failure.
 */
int list_insert(list_t list, int key, point_t* elem);



/* Remove an element from list
 *
 * Searches for an element with a specified key and removes it
 * the element is in the list. 
 *
 * list - the list to remove from
 * key  - the key to find the element by
 * elem - if not NULL, elem will be set to the element if found
 *
 * Returns 1 if the element is found and removed, 0 otherwise.
 */
int list_remove(list_t list, int key, point_t** elem);



/* Clean up list
 *
 * Deletes the list and clean up the resources.
 *
 * list - the list to destroy
 * cb   - user-defined callback or NULL
 *
 * NB! The user-defined callback must manually free element data.
 *     If NULL is given, then free() is called on the element data.
 *
 * Returns a negative value on failure.
 */
int list_free(list_t list, callback_t cb);

#endif
