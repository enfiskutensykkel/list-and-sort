#include <stdio.h>
#include "common.h"
#include "heap.h"

void print_point(point_t* point)
{
    printf("(%d,%d) has cost %d\n", point->x, point->y, point->cost);
}

int main()
{
    heap_t heap;
    heap_create(&heap, 10);

    // Insert elements
    //  9  8  7
    //  6  5  4
    //  3  2  1
    for (int i = 0; i < 9; ++i)
    {
        point_t* point = (point_t*) malloc(sizeof(point_t));
        point->x = i % 3;
        point->y = i / 3;

        int key = 9 - i;

        point->cost = key;

        heap_insert(heap, key, point);
        print_point(point);
    }

    printf("\n");

    while (heap_size(heap) > 0)
    {
        point_t* point;
        heap_remove(heap, &point);
        print_point(point);
        free(point);
    }

    heap_free(heap, NULL);

    return 0;
}
