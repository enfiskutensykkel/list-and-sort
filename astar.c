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
int costs[5][5] = {
    // y -->
    {0, 4, 2, 1, 2}, 
    {1, 4, 3, 1, 1}, 
    {9, 3, 1, 2, 1}, 
    {3, 4, 6, 8, 9},
    {5, 6, 1, 1, 1}
};

// Heuristic cost estimates
int f_score[5][5];


int heuristic_cost_estimate(point_t* start, point_t* goal)
{
    // TODO: Make some sane heuristic that doesn't rely on diagonal path
    return (goal->x - start->y) + (goal->y - start->y);
}


// Implementation from:
// https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
int a_star(list_t graph, int width, int height, point_t* start, point_t* goal)
{
    list_t closed;
    list_create(&closed, width * height);

    start->dist = 0;
    f_score[start->x][start->y] = start->dist + heuristic_cost_estimate(start, goal);

    heap_t open;
    heap_create(&open, width * height);

    heap_insert(open, 0, start);

    while (heap_size(open) > 0)
    {
        // find node with minimal f_score value
        point_t* current;
        heap_remove(open, &current);

        if (current == goal)
        {
            list_free(closed, NULL, NULL);
            heap_free(open, NULL, NULL);
            return EXIT_SUCCESS;
        }

        // remove from open list and insert into closed list
        list_insert(closed, current->id, current);

        // for neighbours of current
        for (int i = MAX(current->x - 1, 0); i <= MIN(current->x + 1, width - 1); ++i)
        {
            for (int j = MAX(current->y - 1, 0); j <= MIN(current->y + 1, height - 1); ++j)
            {
                if (i != current->x || j!= current->y) // skip self
                {
                    point_t* neighbour = graph_find(graph, width, height, i, j);

                    if (list_search(closed, neighbour->id, NULL))
                    {
                        continue; // ignore neighbour which is already evaluated
                    }

                    int tentative_g_score = current->dist + neighbour->cost;

                    if (tentative_g_score < neighbour->dist)
                    {
                        neighbour->prev = current;
                        neighbour->dist = tentative_g_score;
                        f_score[i][j] = neighbour->dist + heuristic_cost_estimate(neighbour, goal);

                        heap_insert(open, neighbour->dist, neighbour);
                    }
                }
            }
        }
    }

    list_free(closed, NULL, NULL);
    heap_free(open, NULL, NULL);
    return EXIT_FAILURE;
}


static void print_point(point_t* v)
{
    printf("(%d,%d)\n", v->x, v->y);
}


static void count_heuristics(point_t* v, int id, void* data)
{
    int* counter = (int*) data;
    *counter += f_score[v->x][v->y];
}


int main()
{
    list_t graph;
    list_create(&graph, 5 * 5);

    for (int x = 0; x < 5; ++x)
    {
        for (int y = 0; y < 5; ++y)
        {
            point_t* vertex = graph_vertex(graph, 5, 5, x, y, costs[x][y]);

            printf("inserting vertex id=%d\n", vertex->id);
        }
    }

    point_t* start = graph_find(graph, 5, 5, 0, 0);
    point_t* goal = graph_find(graph, 5, 5, 4, 4);

    a_star(graph, 5, 5, start, goal);

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

    int total_heuristic = 0;
    graph_traverse(start, goal, (list_cb_t) &count_heuristics, &total_heuristic);

    printf("Total heuristic: %d\n", total_heuristic);

    list_free(graph, (list_cb_t) &free, NULL);

    return 0;
}
