#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "common.h"
#include "list.h"
#include "heap.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Find shortest path from (0,0) to (4,4)
// Should be (0,0) -> (1,0) -> (2,1) -> (3,2) -> (4,3) -> (4,4)
// Total cost = 0 + 1 + 3 + 4 + 1 + 1 = 10
int costs[5][5] = {
    // y -->
    {0, 4, 2, 1, 2}, 
    {1, 4, 3, 1, 1}, 
    {9, 3, 1, 2, 1}, 
    {3, 4, 6, 8, 9},
    {5, 6, 1, 1, 1}
};

int distance[5][5];

point_t *prev[5][5];

void init_graph(point_t* vertex)
{
    distance[vertex->x][vertex->y] = INT_MAX;
    prev[vertex->x][vertex->y] = NULL;
}

void dijkstra(list_t graph, int width, int height, int source_id)
{
    list_walk(graph, (list_cb_t) &init_graph);

    point_t* source;
    list_search(graph, source_id, &source);

    distance[source->x][source->y] = 0;

    heap_t queue;
    heap_create(&queue, width * height);

    heap_insert(queue, 0, source);

    while (heap_size(queue) > 0)
    {
        point_t* u;
        heap_remove(queue, &u);

        for (int i = MAX(u->x - 1, 0); i <= MIN(u->x + 1, width - 1); ++i)
        {
            for (int j = MAX(u->y - 1, 0); j <= MIN(u->y + 1, height - 1); ++j)
            {
                if (i != u->x || j != u->y)
                {
                    int alt = distance[u->x][u->y] + costs[i][j];

                    if (alt < distance[i][j])
                    {
                        distance[i][j] = alt;
                        prev[i][j] = u;

                        point_t* v;
                        list_search(graph, (i * width) + j, &v);

                        heap_insert(queue, alt, v);
                    }
                }
            }
        }
    }

    heap_free(queue, NULL);
}

int main()
{
    list_t graph;
    list_create(&graph, 5 * 5);
    
    for (int x = 0; x < 5; ++x)
    {
        for (int y = 0; y < 5; ++y)
        {
            point_t* vertex = (point_t*) malloc(sizeof(point_t));
            vertex->x = x;
            vertex->y = y;
            vertex->cost = costs[x][y];

            printf("inserting vertex id=%d\n", (x * 5) + y);
            list_insert(graph, (x * 5) + y, vertex);
        }
    }

    dijkstra(graph, 5, 5, 0);

    point_t* ptr = prev[4][4];

    int total = 0;
    while (ptr != NULL)
    {
        printf("(%d,%d) %d\n", ptr->x, ptr->y, ptr->cost);
        total += ptr->cost;
        ptr = prev[ptr->x][ptr->y];
    }
    printf("Total: %d\n", total);

    list_free(graph, (list_cb_t) &free);

    return 0;
}
