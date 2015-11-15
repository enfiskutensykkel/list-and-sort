#include <stdlib.h>
#include <errno.h>
#include "list.h"
#include "common.h"


/* Wrapper for list element entries */
typedef struct {
    int key;
    point_t* point;
} elem_t;


/* List handle type */
struct list {
    elem_t* list;
    size_t elements;
    size_t max_size;
    int locked; // FIXME: Use some atomic compare-and-swap to ensure thread-safety
};


/* Sort list using insertion sort
 * Insertion sort is good for nearly-sorted list.
 * Since we maintain sorting for all inserts and removes,
 * it should perform fairly good
 */
static void list_sort(list_t list)
{
    for (size_t i = 1; i < list->elements; ++i)
    {
        elem_t tmp = list->list[i];
        size_t j = i;
        
        for (j = i; j > 0 && list->list[j - 1].key > tmp.key; --j)
        {
            list->list[j] = list->list[j - 1];
        }

        list->list[j] = tmp;
    }
}

/* Find an element in the list using binary search */
static size_t list_find(list_t list, int key)
{
    size_t min = 0, max = list->elements - 1;

    while (min <= max)
    {
        size_t mid = min + (max - min) / 2;

        if (list->list[mid].key == key)
        {
            return mid;
        }
        else if (list->list[mid].key < key)
        {
            min = mid + 1;
        }
        else 
        {
            max = mid - 1;
        }
    }

    return list->max_size;
}


int list_create(list_t* list, size_t init_size)
{
    *list = (list_t) malloc(sizeof(struct list));
    if (*list == NULL) 
    {
        return -ENOMEM;
    }

    (*list)->list = (elem_t*) malloc(sizeof(elem_t) * init_size);
    if ((*list)->list == NULL)
    {
        free(*list);
        *list = NULL;
        return -ENOMEM;
    }

    (*list)->elements = 0;
    (*list)->max_size = init_size;
    (*list)->locked = 0;

    return 1;
}


int list_free(list_t list, list_cb_t cb, void* data)
{
    if (list->locked)
    {
        return -EBUSY;
    }
    list->locked = 1;

    if (list->list != NULL)
    {
        if (cb != NULL)
        {
            for (size_t i = 0; i < list->elements; ++i)
            {
                cb(list->list[i].point, list->list[i].key, data);
            }
        }

        free(list->list);
        list->list = NULL;
        list->elements = list->max_size = 0;
    }

    free(list);
    return 1;

}


int list_insert(list_t list, int key, point_t* elem)
{
    if (list->locked)
    {
        return -EBUSY;
    }
    list->locked = 1;

    if (list->elements == list->max_size - 1)
    {
        size_t max_size = list->max_size * 2;
        elem_t* new_list = (elem_t*) realloc((void*) list->list, sizeof(elem_t) * max_size);

        if (new_list == NULL) 
        {
            list->locked = 0;
            return -ENOMEM;
        }

        list->list = new_list;
        list->max_size = max_size;
    }

    list->list[list->elements].key = key;
    list->list[list->elements].point = elem;
    list->elements++;

    list_sort(list);

    list->locked = 0;
    return 1;
}


int list_remove(list_t list, int key, point_t** elem)
{
    if (list->locked)
    {
        return -EBUSY;
    }
    list->locked = 1;

    size_t i = list_find(list, key);

    if (i != list->max_size)
    {
        if (elem != NULL)
        {
            *elem = list->list[i].point;
        }

        list->list[i] = list->list[--list->elements];
        list_sort(list);

        list->locked = 0;
        return 1;
    }

    if (elem != NULL)
    {
        *elem = NULL;
    }

    list->locked = 0;
    return 0;
}


int list_search(list_t list, int key, point_t** elem)
{
    if (list->locked)
    {
        return -EBUSY;
    }
    list->locked = 1;

    size_t i = list_find(list, key);

    if (i != list->max_size)
    {
        if (elem != NULL)
        {
            *elem = list->list[i].point;
        }
        
        list->locked = 0;
        return 1;
    }

    if (elem != NULL)
    {
        *elem = NULL;
    }

    list->locked = 0;
    return 0;
}


int list_walk(list_t list, list_cb_t cb, void* data)
{
    if (list->locked)
    {
        return -EBUSY;
    }
    list->locked = 1;

    for (size_t i = 0; i < list->elements; ++i)
    {
        cb(list->list[i].point, list->list[i].key, data);
    }

    list->locked = 0;
    return 1;
}


size_t list_size(list_t list)
{
    return list->elements;
}
