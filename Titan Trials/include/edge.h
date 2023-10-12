#ifndef RAYLIB_H
#include "raylib.h"
#endif

#ifndef EDGE_H
#define EDGE_H

typedef struct Edge2 {
    Vector2 start;
    Vector2 end;
} Edge2;

Edge2   Edge2Initialize(Vector2 start, Vector2 end);
Vector2 Edge2GetNormal(Edge2 edge);

#endif
