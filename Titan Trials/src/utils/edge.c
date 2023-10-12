#include "edge.h"

Edge2 Edge2Initialize(Vector2 start, Vector2 end)
{
    Edge2 edge = { 0 };
    edge.start = start;
    edge.end = end;
    return edge;
}

Vector2 Edge2GetNormal(Edge2 edge)
{
    Vector2 normal = { 0 };
    normal.x = edge.end.y - edge.start.y;
    normal.y = edge.start.x - edge.end.x;
    return normal;
}
