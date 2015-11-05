#include <stdio.h>
#include "common.h"
#include "list.h"

void print_point(int key, point_t* point)
{
    printf("point %d: (%d,%d) has cost %d\n", key, point->x, point->y, point->cost);
}

void change_cost(int key, point_t* point)
{
    point->cost = key * 2;
}

void free_point(int key, point_t* point)
{
    printf("freeing point %d\n", key);
    free(point);
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
        print_point(9 - i, point);
    }

    // Find element
    printf("\n");
    point_t* element;
    list_search(list, 2, &element);
    print_point(2, element);

    // Traverse list
    printf("\n");
    list_travel(list, &print_point);

    // Remove element
    list_remove(list, 2, &element);
    printf("removed element has cost %d\n", element->cost);
    free(element);

    // Traverse list
    printf("\n");
    list_travel(list, &print_point);

    // Traverse list and change cost
    printf("\n");
    list_travel(list, &change_cost);
    list_travel(list, &print_point);

    // Clean up
    printf("\n");
    list_delete(list, &free_point);

    return 0;
}
