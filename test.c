#include <stdio.h>
#include "common.h"
#include "list.h"

void my_callback(int key, point_t* point)
{
    printf("point %d: (%d,%d) has cost %d\n", key, point->x, point->y, point->cost);
}

void my_other_callback(int key, point_t* point)
{
    point->cost = key * 2;
}

int main() 
{
    // Create list
    list_t list;
    list_create(&list, 5);

    // Insert elements
    //  9  8  7
    //  6  5  4
    //  3  2  1
    for (int i = 0; i < 9; ++i)
    {
        point_t* point = (point_t*) malloc(sizeof(point_t));
        point->x = i % 3;
        point->y = i / 3;
        point->cost = i;

        list_insert(list, 9 - i, point);
        my_callback(9 - i, point);
    }

    // Find element
    printf("\n");
    point_t* element;
    list_search(list, 2, &element);
    my_callback(2, element);

    // Traverse list
    printf("\n");
    list_travel(list, &my_callback);

    // Remove element
    list_remove(list, 2, &element);
    printf("removed element has cost %d\n", element->cost);
    free(element);

    // Traverse list
    printf("\n");
    list_travel(list, &my_callback);

    // Traverse list and change cost
    printf("\n");
    list_travel(list, &my_other_callback);
    list_travel(list, &my_callback);

    // Clean up
    list_delete(list);

    return 0;
}
