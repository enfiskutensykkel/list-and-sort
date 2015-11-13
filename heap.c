#include <stdlib.h>
#include <errno.h>
#include "heap.h"
#include "common.h"


/* Wrapper for heap element entries */
typedef struct {
    int key;
    point_t* point;
} elem_t;


/* Heap handle type */
struct heap {
    elem_t* heap;
    size_t elements;
    size_t max_size;
    int locked; // FIXME: Use some atomic operation to ensure thread-safety
};


static size_t percolate_up(heap_t heap, int key)
{
    size_t hole = heap->elements;

    for (; hole > 1 && key < heap->heap[hole / 2].key; hole /= 2)
    {
        heap->heap[hole] = heap->heap[hole / 2];
    }

    return hole;
}


static void percolate_down(heap_t heap)
{
    size_t hole = 1;
    size_t child;

    int key = heap->heap[1].key;
    point_t* ptr = heap->heap[1].point;

    for (; hole * 2 <= heap->elements; hole = child)
    {
        child = hole * 2;

        if (child != heap->elements && heap->heap[child + 1].key < heap->heap[child].key)
        {
            ++child;
        }

        if (heap->heap[child].key < key)
        {
            heap->heap[hole].key = heap->heap[child].key;
            heap->heap[hole].point = heap->heap[child].point;
        }
        else
        {
            break;
        }
    }

    heap->heap[hole].key = key;
    heap->heap[hole].point = ptr;
}


int heap_create(heap_t* heap, size_t init_size)
{
    *heap = (heap_t) malloc(sizeof(struct heap));
    if (*heap == NULL)
    {
        return -ENOMEM;
    }

    (*heap)->heap = (elem_t*) malloc(sizeof(elem_t) * init_size);
    if ((*heap)->heap == NULL)
    {
        free(*heap);
        *heap = NULL;
        return -ENOMEM;
    }

    (*heap)->elements = 0;
    (*heap)->max_size = init_size;
    (*heap)->locked = 0;

    return 1;
}


int heap_free(heap_t heap, heap_cb_t cb)
{
    if (heap->locked)
    {
        return -EBUSY;
    }
    heap->locked = 1;

    if (heap->heap != NULL)
    {
        if (cb == NULL)
        {
            cb = (heap_cb_t) &free;
        }

        for (size_t i = 0; i < heap->elements; ++i)
        {
            cb(heap->heap[i].point);
        }

        free(heap->heap);
        heap->heap = NULL;
        heap->elements = heap->max_size = 0;
    }

    free(heap);
    return 1;
}


int heap_insert(heap_t heap, int key, point_t* elem)
{
    if (heap->locked)
    {
        return -EBUSY;
    }

    if (heap->elements == heap->max_size - 1)
    {
        size_t max_size = heap->max_size * 2;
        elem_t* new_heap = (elem_t*) realloc((void*) heap->heap, sizeof(elem_t) * max_size);

        if (new_heap == NULL)
        {
            heap->locked = 0;
            return -ENOMEM;
        }

        heap->heap = new_heap;
        heap->max_size = max_size;
    }

    ++heap->elements;
    size_t hole = percolate_up(heap, key);

    heap->heap[hole].key = key;
    heap->heap[hole].point = elem;

    heap->locked = 0;
    return 1;
}


int heap_remove(heap_t heap, point_t** elem)
{
    if (heap->locked)
    {
        return -EBUSY;
    }
    heap->locked = 1;

    point_t* remove = NULL;
    if (heap->elements > 0)
    {
        remove = heap->heap[1].point;
        heap->heap[1] = heap->heap[heap->elements--];
        percolate_down(heap);
    }

    if (elem != NULL)
    {
        *elem = remove;
    }
    heap->locked = 0;
    return remove != NULL;
}


size_t heap_size(heap_t heap)
{
    return heap->elements;
}
