#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "common.h"
#include "list.h"



/* Create graph vertex
 *
 * Create a point in a grid represented by a vertex.
 *
 * g    - the graph to insert the vertex into
 * w    - the grid width
 * h    - the grid height
 * x    - the X coordinate of the point
 * y    - the Y coordinate of the point
 * cost - the weight/cost of the point
 *
 * Returns the newly created point or NULL on error.
 */
point_t* graph_vertex(list_t g, int w, int h, int x, int y, int cost);



/* Retrieve vertex
 *
 * Get a point in the graph by (X, Y) coordinates.
 *
 * g    - the graph to search in
 * w    - the graph width
 * h    - the graph height
 * x    - the X coordinate of the point
 * y    - the Y coordinate of the point
 */
point_t* graph_find(list_t g, int w, int h, int x, int y);



/* Calculate path distance
 *
 * Calculate path distance between point a and point b.
 *
 * a    - starting point
 * b    - the point to calculate distance to
 *
 * NB! It is important that the prev path is calculated properly before
 *     invoking this function. If the prev path from b doesn't lead to a,
 *     then this function will have undefined behaviour.
 *
 * Returns the distance from point a to point b in form of accumulated cost.
 */
int graph_distance(point_t* a, point_t* b);



/* Traverse path
 *
 * Traverse the path between point a and point b and call a user-supplied callback.
 *
 * a    - starting point
 * b    - the point to traverse to
 * cb   - user-defined callback
 * data - user-supplied data
 *
 * NB! It is important that the prev path is calculated properly before
 *     invoking this function. If the prev path from b doesn't lead to a,
 *     then this function will have undefined behaviour.
 */
void graph_traverse(point_t* a, point_t* b, list_cb_t cb, void* data);

#endif
