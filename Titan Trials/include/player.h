#pragma once

#include "raylib.h"
#include "block.h"
#include "stdbool.h"

typedef struct PlayerPosition {
    Vector2 previousCoordinates;
    Vector2 coordinates;
    Vector2 nextCoordinates;
    Vector2 size;
    Vector2 direction;
    Rectangle hitbox;
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
    Color color;
} PlayerAttributes;

typedef struct Player {
    PlayerPosition position;
    PlayerPhysics physics;
    PlayerAttributes attributes;
} Player;

Player initializePlayer(float x, float y, float width, float height);
void updatePlayer(Player *player, Block blocks[], size_t block_count, float deltaTime);
void drawPlayer(Player player);
void updateDirection(Player *player);
void handleBlockCollision(Player *player, Block block);
void handlePlayerInput(Player *player, float deltaTime);

