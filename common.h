#ifndef __COMMON_H__
#define __COMMON_H__

typedef struct point point_t;

struct point 
{
    int      id;    // Unique identifier
    int      x, y;  // Coordinates
    int      cost;  // Cost of going to point
    int      dist;  // Distance to point
    point_t* prev;  // Previous point in path
};

#endif
