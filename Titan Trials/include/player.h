#ifndef RAYLIB_H
#include "raylib.h"
#endif

#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "polygon.h"

typedef struct PlayerPosition {
    ConvexPolygon polygon;
    Vector2 previousCoordinates;
    Vector2 coordinates;
    Vector2 nextCoordinates;
    Vector2 direction;
} PlayerPosition;

typedef struct PlayerPhysics {
    Vector2 velocity;
    Vector2 force;
    Vector2 acceleration;
    float deceleration;
    float mass;
} PlayerPhysics;

typedef struct PlayerAttributes {
    float movementSpeed;
    float jumpPower;
    bool canJump;
} PlayerAttributes;

typedef struct Player {
    PlayerPosition position;
    PlayerPhysics physics;
    PlayerAttributes attributes;
} Player;

Player PlayerInitialize(ConvexPolygon polygon);
void   PlayerUpdate(Player *player, float deltaTime);
void   PlayerDraw(Player *player);
void   PlayerUpdateDirection(Player *player);
void   PlayerHandleInput(Player *player, float deltaTime);

#endif
