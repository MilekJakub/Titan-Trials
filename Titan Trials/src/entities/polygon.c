#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "polygon.h"
#include "raymath.h"
#include "more_math.h"

ConvexPolygon PolygonInitializeRectangle(float x, float y, float width, float height, Color color)
{
    ConvexPolygon polygon = { 0 };
    polygon.coordinates = (Vector2) { x, y };

    polygon.vertexCount = 4;
    polygon.vertices = malloc(sizeof(Vector2) * polygon.vertexCount);
    polygon.vertices[0] = (Vector2) { x - width / 2, y - height / 2 };
    polygon.vertices[1] = (Vector2) { x + width / 2, y - height / 2 };
    polygon.vertices[2] = (Vector2) { x + width / 2, y + height / 2 };
    polygon.vertices[3] = (Vector2) { x - width / 2, y + height / 2 };

    polygon.edgeCount = 4;
    polygon.edges = malloc(sizeof(Edge2) * polygon.edgeCount);
    polygon.edges[0] = (Edge2) { polygon.vertices[0], polygon.vertices[1] };
    polygon.edges[1] = (Edge2) { polygon.vertices[1], polygon.vertices[2] };
    polygon.edges[2] = (Edge2) { polygon.vertices[2], polygon.vertices[3] };
    polygon.edges[3] = (Edge2) { polygon.vertices[3], polygon.vertices[0] };

    polygon.color = color;

    return polygon;
}

void PolygonRectangleUpdateCoordinates(ConvexPolygon *polygon, float x, float y)
{
    float xDiff = polygon->coordinates.x - x; 
    float yDiff = polygon->coordinates.y - y; 

    polygon->coordinates = (Vector2) { x, y };

    polygon->vertices[0] = (Vector2) { polygon->vertices[0].x - xDiff, polygon->vertices[0].y - yDiff };
    polygon->vertices[1] = (Vector2) { polygon->vertices[1].x - xDiff, polygon->vertices[1].y - yDiff };
    polygon->vertices[2] = (Vector2) { polygon->vertices[2].x - xDiff, polygon->vertices[2].y - yDiff };
    polygon->vertices[3] = (Vector2) { polygon->vertices[3].x - xDiff, polygon->vertices[3].y - yDiff };

    polygon->edges[0] = (Edge2) { polygon->vertices[0], polygon->vertices[1] };
    polygon->edges[1] = (Edge2) { polygon->vertices[1], polygon->vertices[2] };
    polygon->edges[2] = (Edge2) { polygon->vertices[2], polygon->vertices[3] };
    polygon->edges[3] = (Edge2) { polygon->vertices[3], polygon->vertices[0] };
}


ConvexPolygon PolygonInitializeTriangle(Vector2 a, Vector2 b, Vector2 c, Color color)
{
    ConvexPolygon polygon = { 0 };
    polygon.coordinates = (Vector2) { (a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3 };

    polygon.vertexCount = 3;
    polygon.vertices = malloc(sizeof(Vector2) * polygon.vertexCount);
    polygon.vertices[0] = a;
    polygon.vertices[1] = b;
    polygon.vertices[2] = c;

    polygon.edgeCount = 3;
    polygon.edges = malloc(sizeof(Edge2) * polygon.edgeCount);
    polygon.edges[0] = (Edge2) { polygon.vertices[0], polygon.vertices[1] };
    polygon.edges[1] = (Edge2) { polygon.vertices[1], polygon.vertices[2] };
    polygon.edges[2] = (Edge2) { polygon.vertices[2], polygon.vertices[0] };

    polygon.color = color;

    return polygon;
}

void PolygonTriangleUpdateCoordinates(ConvexPolygon *polygon, float x, float y)
{
    float xDiff = polygon->coordinates.x - x; 
    float yDiff = polygon->coordinates.y - y; 

    polygon->coordinates = (Vector2) { x, y };

    polygon->vertices[0] = (Vector2) { polygon->vertices[0].x - xDiff, polygon->vertices[0].y - yDiff };
    polygon->vertices[1] = (Vector2) { polygon->vertices[1].x - xDiff, polygon->vertices[1].y - yDiff };
    polygon->vertices[2] = (Vector2) { polygon->vertices[2].x - xDiff, polygon->vertices[2].y - yDiff };

    polygon->edges[0] = (Edge2) { polygon->vertices[0], polygon->vertices[1] };
    polygon->edges[1] = (Edge2) { polygon->vertices[1], polygon->vertices[2] };
    polygon->edges[2] = (Edge2) { polygon->vertices[2], polygon->vertices[0] };
}

bool PolygonSeparationConvexConvex(ConvexPolygon *a, ConvexPolygon *b)
{
    Vector2 *axes = PolygonGetAxes(a);
    size_t axisCount = a->vertexCount;

    for (size_t i = 0; i < axisCount; i++)
    {
        Vector2 axis = axes[i];
        Vector2 p1 = PolygonProjection(a, axis);
        Vector2 p2 = PolygonProjection(b, axis);
        if (!Vector2Overlap(p1, p2))
        {
            return false;
        }
    }

    return true;
}

Vector2 *PolygonGetAxes(ConvexPolygon *polygon)
{
    Vector2 *axes = malloc(sizeof(Vector2) * polygon->vertexCount);

    for (int i = 0; i < polygon->vertexCount; i++)
    {
        Vector2 p1 = polygon->vertices[i];
        Vector2 p2 = polygon->vertices[i + 1 == polygon->vertexCount ? 0 : i + 1];
        Vector2 edge = Vector2Subtract(p1, p2);
        Vector2 normal = Vector2Perpendicular(edge);
        axes[i] = normal;
    }

    return axes;
}

Vector2 PolygonProjection(ConvexPolygon *polygon, Vector2 axis)
{
    float min = Vector2DotProduct(axis, polygon->vertices[0]);
    float max = min;

    for (size_t i = 1; i < polygon->vertexCount; i++)
    {
        // NOTE: The axis must be normalized to get accurate projections.
        float p = Vector2DotProduct(axis, polygon->vertices[i]);
        if (p < min)
        {
            min = p;
        }
        else if (p > max)
        {
            max = p;
        }
    }

    Vector2 projection = (Vector2) { min, max };
    return projection;
}

void PolygonHandleCollisionConvexConvex(ConvexPolygon *a, ConvexPolygon *b)
{
    if (PolygonSeparationConvexConvex(a, b) && PolygonSeparationConvexConvex(b, a))
    {
        // Step 1: For each polygon, find the two farthest points along the projection of that polygon onto the line perpendicular to the motion vector.
        // Step 2: Divide each polygon along the line connecting these points. The half of the polygon that faces the other polygon along the motion vector is the "forward hull". This is the only part of the polygon that can possibly collide.
        // Step 3: Project a vector from each point on each polygon's "forward hull" along the motion vector towards the opposite polygon, and check it for intersection with each edge of the opposite polygon's "forward hull".
        // Step 4: Take the shortest vector. This is the exact collision distance.
        // Step 5: Move the polygons apart by the collision distance.
        DrawText("Collision!", 500, 100, 20, WHITE);
    }
}

void PolygonDrawConvex(ConvexPolygon *polygon)
{
    rlBegin(RL_LINES);

        rlColor4ub(polygon->color.r, polygon->color.g, polygon->color.b, polygon->color.a);

        for (int i = 0; i < polygon->edgeCount; i++)
        {
            rlVertex2f(polygon->edges[i].start.x, polygon->edges[i].start.y);
            rlVertex2f(polygon->edges[i].end.x, polygon->edges[i].end.y);
        }

    rlEnd();
}

void PolygonDrawArrayConvex(ConvexPolygon *polygons, size_t count)
{
    for (int i = 0; i < count; i++)
    {
        PolygonDrawConvex(&polygons[i]);
        PolygonShowVerticesConvex(&polygons[i]);
    }
}

void PolygonFreeConvex(ConvexPolygon *polygon)
{
    free(polygon->vertices);
    free(polygon->edges);
}


void PolygonRotateConvex(ConvexPolygon *polygon, float degrees)
{
    for (int i = 0; i < polygon->vertexCount; i++)
    {
        polygon->vertices[i] = Vector2Subtract(polygon->vertices[i], polygon->coordinates);
        polygon->vertices[i] = Vector2Rotate(polygon->vertices[i], degrees);
        polygon->vertices[i] = Vector2Add(polygon->vertices[i], polygon->coordinates);
    }

    for (int i = 0; i < polygon->edgeCount; i++)
    {
        polygon->edges[i].start = polygon->vertices[i];
        if (i + 1 == polygon->edgeCount)
        {
            polygon->edges[i].end = polygon->vertices[0];
        }
        else
        {
            polygon->edges[i].end = polygon->vertices[i + 1];
        }
    }

}

ConvexPolygon PolygonInitializeConvex(Vector2 *vertices, size_t vertexCount, Color color)
{
    ConvexPolygon polygon = { 0 };
    float polygonX = 0.0f;
    float polygonY = 0.0f;

    for (size_t i = 0; i < vertexCount; i++)
    {
        polygonX += vertices[i].x;
        polygonY += vertices[i].y;
    }

    polygonX /= vertexCount;
    polygonY /= vertexCount;

    polygon.coordinates = (Vector2) { polygonX, polygonY };

    polygon.vertexCount = vertexCount;
    polygon.vertices = malloc(sizeof(Vector2) * polygon.vertexCount);
    for (size_t i = 0; i < vertexCount; i++)
    {
        polygon.vertices[i] = vertices[i];
    }

    polygon.edgeCount = vertexCount;
    polygon.edges = malloc(sizeof(Edge2) * polygon.edgeCount);
    for (size_t i = 0; i < vertexCount; i++)
    {
        polygon.edges[i].start = polygon.vertices[i];
        if (i + 1 == polygon.edgeCount)
        {
            polygon.edges[i].end = polygon.vertices[0];
        }
        else
        {
            polygon.edges[i].end = polygon.vertices[i + 1];
        }
    }

    polygon.color = color;

    return polygon;
}

void PolygonUpdateCoordinatesConvex(ConvexPolygon *polygon, float x, float y)
{
    float xDiff = polygon->coordinates.x - x; 
    float yDiff = polygon->coordinates.y - y; 

    polygon->coordinates = (Vector2) { x, y };

    for (size_t i = 0; i < polygon->vertexCount; i++)
    {
        polygon->vertices[i] = (Vector2) { polygon->vertices[i].x - xDiff, polygon->vertices[i].y - yDiff };
    }

    for (size_t i = 0; i < polygon->edgeCount; i++)
    {
        polygon->edges[i].start = polygon->vertices[i];
        if (i + 1 == polygon->edgeCount)
        {
            polygon->edges[i].end = polygon->vertices[0];
        }
        else
        {
            polygon->edges[i].end = polygon->vertices[i + 1];
        }
    }
}

void PolygonShowVerticesConvex(ConvexPolygon *polygon)
{
    DrawCircle(polygon->coordinates.x, polygon->coordinates.y, 2.0f, GREEN);
    for (int i = 0; i < polygon->vertexCount; i++)
    {
        DrawCircle(polygon->vertices[i].x, polygon->vertices[i].y, 2.0f, GREEN);
    }
}

ConcavePolygon PolygonInitializeConcave(ConvexPolygon *polygons, size_t polygonsCount, Color color)
{
    size_t vertexCount = 0;

    for (size_t i = 0; i < polygonsCount; i++)
    {
        vertexCount += polygons[i].vertexCount;
    }

    size_t edgeCount = 0;

    for (size_t i = 0; i < polygonsCount; i++)
    {
        edgeCount += polygons[i].edgeCount;
    }

    ConcavePolygon polygon = { 0 };

    float polygonX = 0.0f;
    float polygonY = 0.0f;
    
    for (size_t i = 0; i < polygonsCount; i++)
    {
        for (size_t j = 0; j < polygons[i].vertexCount; j++)
        {
            polygonX += polygons[i].vertices[j].x;
            polygonY += polygons[i].vertices[j].y;
        }
    }

    polygon.coordinates = (Vector2) { polygonX /= vertexCount, polygonY /= vertexCount };
    polygon.vertexCount = vertexCount;
    polygon.edgeCount = edgeCount;
    polygon.convexPolygons = malloc(sizeof(ConvexPolygon) * polygonsCount);
    polygon.convexPolygonCount = polygonsCount;

    for (size_t i = 0; i < polygonsCount; i++)
    {
        polygon.convexPolygons[i] = polygons[i];
    }

    polygon.color = color;

    return polygon;
}   

void PolygonUpdateCoordinatesConcave(ConcavePolygon *polygon, float x, float y)
{
    float xDiff = polygon->coordinates.x - x; 
    float yDiff = polygon->coordinates.y - y; 

    polygon->coordinates = (Vector2) { x, y };

    for (size_t i = 0; i < polygon->convexPolygonCount; i++)
    {
        PolygonUpdateCoordinatesConvex(&polygon->convexPolygons[i], x, y);
    }
}

void PolygonDrawConcave(ConcavePolygon *polygon)
{
    for (size_t i = 0; i < polygon->convexPolygonCount; i++)
    {
        PolygonDrawConvex(&polygon->convexPolygons[i]);
    }
}

void PolygonDrawArrayConcave(ConcavePolygon *polygons, size_t count)
{
    for (int i = 0; i < count; i++)
    {
        PolygonDrawConcave(&polygons[i]);
        PolygonShowVerticesConcave(&polygons[i]);
    }
}

void PolygonFreeConcave(ConcavePolygon *polygon)
{
    free(polygon->convexPolygons);
}

void PolygonRotateConcave(ConcavePolygon *polygon, float degrees)
{
    for (size_t i = 0; i < polygon->convexPolygonCount; i++)
    {
        for (int j = 0; j < polygon->convexPolygons[i].vertexCount; j++)
        {
            polygon->convexPolygons[i].vertices[j] = Vector2Subtract(polygon->convexPolygons[i].vertices[j], polygon->coordinates);
            polygon->convexPolygons[i].vertices[j] = Vector2Rotate(polygon->convexPolygons[i].vertices[j], degrees);
            polygon->convexPolygons[i].vertices[j] = Vector2Add(polygon->convexPolygons[i].vertices[j], polygon->coordinates);
        }
    }

    for (size_t i = 0; i < polygon->convexPolygonCount; i++)
    {
        for (int j = 0; j < polygon->convexPolygons[i].edgeCount; j++)
        {
            polygon->convexPolygons[i].edges[j].start = polygon->convexPolygons[i].vertices[j];

            if (j + 1 == polygon->convexPolygons[i].edgeCount)
            {
                polygon->convexPolygons[i].edges[j].end = polygon->convexPolygons[i].vertices[0];
            }
            else
            {
                polygon->convexPolygons[i].edges[j].end = polygon->convexPolygons[i].vertices[j + 1];
            }
        }
    }
}

bool PolygonSeparationConcaveConvex(ConcavePolygon *a, ConvexPolygon *b)
{
    for (size_t i = 0; i < a->convexPolygonCount; i++)
    {
        if (PolygonSeparationConvexConvex(&a->convexPolygons[i], b))
        {
            return true;
        }
    }

    return false;
}

bool PolygonSeparationConcaveConcave(ConcavePolygon *a, ConcavePolygon *b)
{
    for (size_t i = 0; i < a->convexPolygonCount; i++)
    {
        for (size_t j = 0; j < b->convexPolygonCount; j++)
        {
            if (PolygonSeparationConvexConvex(&a->convexPolygons[i], &b->convexPolygons[j]))
            {
                return true;
            }
        }
    }

    return false;
}

void PolygonShowVerticesConcave(ConcavePolygon *polygon)
{
    DrawCircle(polygon->coordinates.x, polygon->coordinates.y, 2.0f, GREEN);
    for (size_t i = 0; i < polygon->convexPolygonCount; i++)
    {
        for (size_t j = 0; j < polygon->convexPolygons[i].vertexCount; j++)
        {
            DrawCircle(polygon->convexPolygons[i].vertices[j].x, polygon->convexPolygons[i].vertices[j].y, 2.0f, GREEN);
        }
    }
}

void PolygonHandleCollisionConcaveConvex(ConcavePolygon *a, ConvexPolygon *b)
{
    if (PolygonSeparationConcaveConvex(a, b))
    {
        DrawText("Collision!", 500, 100, 20, WHITE);
    }
}

void PolygonHandleCollisionConcaveConcave(ConcavePolygon *a, ConcavePolygon *b)
{
    if (PolygonSeparationConcaveConcave(a, b))
    {
        DrawText("Collision!", 500, 100, 20, WHITE);
    }
}

