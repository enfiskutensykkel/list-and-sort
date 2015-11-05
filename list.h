#ifndef __LIST_H__
#define __LIST_H__

#include "common.h"
#include <stdlib.h>

/* List type */
typedef struct list* list_t;

/* Callback type */
//typedef void (*callback_t)(int key, point_t* elem);
typedef void (*callback_t)(point_t* elem, int key);



/* Initialize a list with a certain size
 * The list will grow when it gets full.
 *
 * Returns 0 on success.
 */
int list_create(list_t* list, size_t initial_size);



/* Find an element in a list
 * Searches for an element with a key equal to key.
 * 
 * Returns 0 if found and sets elem if elem is not NULL.
 */
int list_search(list_t list, int key, point_t** elem);



/* Remove an element from a list
 * Searches for an element with a key equal to key.
 *
 * Returns 0 if found and sets elem to the removed element if elem is not NULL.
 */
int list_remove(list_t list, int key, point_t** elem);



/* Insert an element into a list
 * Inserts an element into a list using key.
 *
 * Returns 0 on success.
 */
int list_insert(list_t list, int key, point_t* elem);



/* Delete a list
 * Deletes the list and cleans up the resources.
 * NB! If list is not empty, the supplied callback must free elements.
 *
 * Returns 0 on success.
 */
int list_delete(list_t list, callback_t cb);



/* Traverse a list
 * Traverses a list and calls a callback function with each element.
 *
 * Returns 0 on success.
 */
int list_travel(list_t list, callback_t cb);

#endif
