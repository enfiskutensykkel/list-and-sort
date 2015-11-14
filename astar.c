#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "common.h"
#include "list.h"
#include "heap.h"

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

int g_score[5][5];
int f_score[5][5];

point_t *prev[5][5];

int heuristic_cost(point_t* start, point_t* goal)
{
    // TODO: Make some heuristic cost estimate
    if (start == goal)
    {
        return 0;
    }

    return 1;
}

int reverse_path(point_t* v, point_t* u)
{
    if (u == v)
    {
        printf("(%d,%d) %d\n", v->x, v->y, v->cost);
        return v->cost + heuristic_cost(v, u);
    }

    int cost = reverse_path(v, prev[u->x][u->y]);
    printf("(%d,%d) %d\n", u->x, u->y, u->cost);
    return cost + u->cost + heuristic_cost(v, u);
}

void init_graph(point_t* vertex)
{
    g_score[vertex->x][vertex->y] = INT_MAX;
    f_score[vertex->x][vertex->y] = INT_MAX;
    prev[vertex->x][vertex->y] = NULL;
}

// Implementation from:
// https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
int a_star(list_t graph, int width, int height, point_t* start, point_t* goal)
{
    list_walk(graph, (list_cb_t) &init_graph);
    
    list_t closed;
    list_create(&closed, width * height);

    g_score[start->x][start->y] = 0;
    f_score[start->x][start->y] = g_score[start->x][start->y] + heuristic_cost(start, goal);

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
            list_free(closed, NULL);
            heap_free(open, NULL);
            return EXIT_SUCCESS;
        }

        // remove from open list and insert into closed list
        list_insert(closed, (current->x * width) + current->y, current);

        // for neighbours of current
        for (int i = MAX(current->x - 1, 0); i <= MIN(current->x + 1, width - 1); ++i)
        {
            for (int j = MAX(current->y - 1, 0); j <= MIN(current->y + 1, height - 1); ++j)
            {
                int neighbour_id = i * width + j;

                if (list_search(closed, neighbour_id, NULL))
                {
                    continue; // ignore neighbour which is already evaluated
                }

                int tentative_g_score = g_score[current->x][current->y] + costs[i][j];
                if (tentative_g_score < g_score[i][j])
                {
                    point_t* neighbour;
                    list_search(graph, neighbour_id, &neighbour);

                    prev[i][j] = current;
                    g_score[i][j] = tentative_g_score;
                    f_score[i][j] = g_score[i][j] + heuristic_cost(neighbour, goal);

                    heap_insert(open, neighbour_id, neighbour);
                }
            }
        }
    }

    list_free(closed, NULL);
    heap_free(open, NULL);
    return EXIT_FAILURE;
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

    point_t* start;
    point_t* end;

    list_search(graph, 0, &start);
    list_search(graph, (4 * 5) + 4, &end);

    a_star(graph, 5, 5, start, end);

    int total = reverse_path(start, end);
    printf("Total cost: %d\n", total);

    list_free(graph, (list_cb_t) &free);

    return 0;
}
