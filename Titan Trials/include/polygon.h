#ifndef RAYLIB_H
#include "raylib.h"
#endif

#ifndef RLGL_H
#include "rlgl.h"
#endif

#ifndef POLYGON_H
#define POLYGON_H

#include "edge.h"

typedef struct ConvexPolygon {
    Vector2 coordinates;
    Vector2 *vertices;
    size_t vertexCount;
    Edge2 *edges;
    size_t edgeCount;
    Color color;
} ConvexPolygon;

// This is a concave polygon, build with convex polygons
typedef struct ConcavePolygon {
    Vector2 coordinates;
    ConvexPolygon *convexPolygons;
    size_t convexPolygonCount;
    size_t vertexCount;
    size_t edgeCount;
    Color color;
} ConcavePolygon;

/* Polygons */
ConvexPolygon  PolygonInitializeConvex(Vector2 *vertices, size_t vertexCount, Color color);
ConcavePolygon PolygonInitializeConcave(ConvexPolygon *polygons, size_t polygonsCount, Color color);
void           PolygonUpdateCoordinatesConvex(ConvexPolygon *polygon, float x, float y);
void           PolygonUpdateCoordinatesConcave(ConcavePolygon *polygon, float x, float y);

/* Triangles */
ConvexPolygon PolygonInitializeTriangle(Vector2 a, Vector2 b, Vector2 c, Color color);
void          PolygonTriangleUpdateCoordinates(ConvexPolygon *polygon, float x, float y);

/* Rectangles */
ConvexPolygon PolygonInitializeRectangle(float x, float y, float width, float height, Color color);
void          PolygonRectangleUpdateCoordinates(ConvexPolygon *polygon, float x, float y);


/* General */
void PolygonDrawConvex(ConvexPolygon *polygon);
void PolygonDrawConcave(ConcavePolygon *polygon);
void PolygonDrawArrayConvex(ConvexPolygon *polygons, size_t count);
void PolygonDrawArrayConcave(ConcavePolygon *polygons, size_t count);
void PolygonFreeConvex(ConvexPolygon *polygon);
void PolygonFreeConcave(ConcavePolygon *polygon);
void PolygonRotateConvex(ConvexPolygon *polygon, float degrees);
void PolygonRotateConcave(ConcavePolygon *polygon, float degrees);

/* Collision Detection */
bool PolygonSeparationConvexConvex(ConvexPolygon *a, ConvexPolygon *b);
bool PolygonSeparationConcaveConvex(ConcavePolygon *a, ConvexPolygon *b);
bool PolygonSeparationConcaveConcave(ConcavePolygon *a, ConcavePolygon *b);

void PolygonHandleCollisionConvexConvex(ConvexPolygon *a, ConvexPolygon *b);
void PolygonHandleCollisionConcaveConvex(ConcavePolygon *a, ConvexPolygon *b);
void PolygonHandleCollisionConcaveConcave(ConcavePolygon *a, ConcavePolygon *b);

Vector2* PolygonGetAxes(ConvexPolygon *polygon);
Vector2  PolygonProjection(ConvexPolygon *polygon, Vector2 axis);

/* Utilities */
void PolygonShowVerticesConvex(ConvexPolygon *polygon);
void PolygonShowVerticesConcave(ConcavePolygon *polygon);

#endif
