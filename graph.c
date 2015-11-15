#include <stdlib.h>
#include <limits.h>
#include "common.h"
#include "list.h"
#include "graph.h"


static int graph_id(int width, int height, int x, int y)
{
    if (y < 0 || y >= height || x < 0 || x >= width)
    {
        return -1;
    }

    return (x * width) + y;
}


static void count_costs(point_t* p, int id, void* accumulated_costs)
{
    *((int*) accumulated_costs) += p->cost;
}


point_t* graph_vertex(list_t graph, int width, int height, int x, int y, int cost)
{
    point_t* point = (point_t*) malloc(sizeof(point_t));
    if (point == NULL)
    {
        return NULL;
    }

    point->id = graph_id(width, height, x, y);
    point->x = x;
    point->y = y;
    point->cost = cost;
    point->dist = INT_MAX;
    point->prev = NULL;

    if (list_insert(graph, point->id, point) < 0)
    {
        free(point);
        return NULL;
    }

    return point;
}


point_t* graph_find(list_t graph, int width, int height, int x, int y)
{
    int id = graph_id(width, height, x, y);

    point_t* point;
    list_search(graph, id, &point); // will set point to NULL if not found

    return point;
}


void graph_traverse(point_t* a, point_t* b, list_cb_t cb, void* data)
{
    if (a != b)
    {
        graph_traverse(a, b->prev, cb, data);
    }

    cb(b, b->id, data);
}


int graph_distance(point_t* a, point_t* b)
{
    int distance = 0;

    graph_traverse(a, b, &count_costs, &distance);

    return distance;
}
