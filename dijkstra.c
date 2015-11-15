#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "common.h"
#include "list.h"
#include "heap.h"
#include "graph.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


// Find shortest path from (0,0) to (4,4)
// Should be    (0,0) -> (1,0) -> (2,1) -> (3,1) -> (4,2) -> (4,3) -> (4,4)
// Total cost =   0   +    1    +   3   +    4    +   1    +   1    +   1   = 11
int costs[5][5] = {
    // y -->
    {0, 4, 2, 1, 2}, 
    {1, 4, 3, 1, 1}, 
    {9, 3, 1, 2, 1}, 
    {3, 4, 6, 8, 9},
    {5, 6, 1, 1, 1}
};



// Implementation from:
// https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Using_a_priority_queue
void dijkstra(list_t graph, int width, int height, point_t* source)
{
    // set distance to self to 0
    source->prev = source;
    source->dist = 0;

    // create priority queue
    heap_t queue;
    heap_create(&queue, width * height);

    heap_insert(queue, 0, source);

    while (heap_size(queue) > 0)
    {
        // find vertex u with minimal cost
        point_t* u;
        heap_remove(queue, &u);

        // for neighbour v to u
        for (int i = MAX(u->x - 1, 0); i <= MIN(u->x + 1, width - 1); ++i)
        {
            for (int j = MAX(u->y - 1, 0); j <= MIN(u->y + 1, height - 1); ++j)
            {
                if (i != u->x || j != u->y) // u can't be its own neighbour
                {
                    point_t* v = graph_find(graph, width, height, i, j);

                    // is the current distance to u shorter through v?
                    int alt = u->dist + v->cost;

                    if (alt < v->dist) 
                    {
                        v->dist = alt;
                        v->prev = u;

                        heap_insert(queue, alt, v);
                    }
                }
            }
        }
    }

    heap_free(queue, NULL, NULL);
}


void print_point(point_t* node)
{
    printf("(%d,%d)\n", node->x, node->y);
}


int main()
{
    list_t graph;
    list_create(&graph, 5 * 5);
    
    for (int x = 0; x < 5; ++x)
    {
        for (int y = 0; y < 5; ++y)
        {
            // sets everything to have a distance of infinity
            point_t* vertex = graph_vertex(graph, 5, 5, x, y, costs[x][y]);

            printf("inserting vertex id=%d\n", vertex->id);
        }
    }

    point_t* start = graph_find(graph, 5, 5, 0, 0);
    point_t* goal = graph_find(graph, 5, 5, 4, 4);

    dijkstra(graph, 5, 5, start);

    printf("Shortest path from (%d,%d) to (%d,%d)\n",
            start->x, start->y,
            goal->x, goal->y
          );

    graph_traverse(start, goal, (list_cb_t) &print_point, NULL);

    printf("Shortest distance from (%d,%d) to (%d,%d) = %d\n", 
            start->x, start->y, 
            goal->x, goal->y, 
            graph_distance(start, goal) // = goal->dist
            );

    list_free(graph, (list_cb_t) &free, NULL);

    return 0;
}
