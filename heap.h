#ifndef __HEAP_H__
#define __HEAP_H__

#include "common.h"
#include <stdlib.h>

/* Heap handle type */
typedef struct heap* heap_t;



/* Callback type
 *
 * Used for freeing up heap resources.
 * 
 * elem - current element
 */
typedef void (*heap_cb_t)(point_t* elem);



/* Initialize heap
 *
 * Initialize a binary heap with an initial capacity.
 *
 * heap - pointer to a heap handle
 * size - initial heap capacity (will grow when needed)
 *
 * Returns a negative value on failure.
 */
int heap_create(heap_t* heap, size_t size);



/* Get the number of elements in heap
 *
 * Returns the number of elements in the specified heap.
 *
 * heap - the heap to count elements in
 *
 * Returns the number of elements in the heap.
 */
size_t heap_size(heap_t heap);



/* Insert an element into heap
 *
 * Insert an element in a heap with a specified key.
 *
 * heap - the heap to insert into
 * key  - the key to use for the element
 * elem - the element to insert
 *
 * Returns a negative value on failure.
 */
int heap_insert(heap_t heap, int key, point_t* elem);



/* Remove an element from heap
 *
 * Removes the element with the lowest key from the heap.
 *
 * heap - the heap to remove from
 * elem - if not NULL, elem will be set to the element with lowest key if the 
 *        heap is non-empty
 *
 * Returns 1 if the heap is non-empty and an element is removed, 0 otherwise.
 */
int heap_remove(heap_t heap, point_t** elem);



/* Clean up heap
 *
 * Deletes the heap and cleans up the resources.
 *
 * heap - the heap to destroy
 * cb   - user-defined callback or NULL
 *
 * NB! The user-defined callback must manually free element data.
 *     If NULL is given, then free() is called on the element.
 *
 * Returns a negative value on failure.
 */
int heap_free(heap_t heap, heap_cb_t cb);

#endif
