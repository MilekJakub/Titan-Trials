#include "more_math.h"

Vector2 Vector2Perpendicular(Vector2 vector)
{
    Vector2 perpendicular = { 0 };
    perpendicular.x = vector.y;
    perpendicular.y = -vector.x;
    return perpendicular;
}

bool Vector2Overlap(Vector2 projection1, Vector2 projection2)
{
    return (projection1.x <= projection2.y && projection1.y >= projection2.x);
}

