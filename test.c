#include <stdio.h>
#include "common.h"
#include "list.h"

int main() 
{
    list_t list;
    list_create(&list, 5);

    for (int i = 0; i < 8; ++i)
    {
        point_t* point = (point_t*) malloc(sizeof(point_t));
        point->x = 0;
        point->y = 0;
        point->cost = i;

        list_insert(list, 8 - i, point);
    }

    point_t* element;
    list_search(list, 4, &element);

    printf("%d\n", element->cost);

    list_delete(list);

    return 0;
}
