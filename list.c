#include <stdlib.h>
#include <errno.h>
#include "common.h"
#include "list.h"

typedef struct {
    int key;
    point_t* point;
} elem_t;

struct list {
    elem_t* list;
    size_t elements;
    size_t max_size;
};

int list_create(list_t* list, size_t init_size)
{
    *list = (list_t) malloc(sizeof(elem_t) * init_size);
    if (*list == NULL) 
    {
        return ENOMEM;
    }

    *list->elements = 0;
    *list->max_size = init_size;

    return 0;
}

int list_insert(list_t list, int key, point_t* elem)
{
    if (list->elements == list->max_size - 1)
    {
        size_t max_size = list->max_size * 2;
        elem_t* new_list = (elem_t*) realloc((void*) list->list, sizeof(elem_t) * max_size);

        if (new_list == NULL) 
        {
            return ENOMEM;
        }

        list->list = new_list;
        list->max_size = max_size;
    }

    list->list[list->elements]->key = key;
    list->list[list->elements]->point = elem;
    list->elements++;

    return 0;
}
