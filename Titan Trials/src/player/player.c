#include "player.h"

#ifndef RAYMATH_H
#include "raymath.h"
#endif

Player PlayerInitialize(ConvexPolygon polygon)
{
    Player player = { 0 };
    PlayerPosition playerPosition = { 0 };
    PlayerPhysics playerPhysics = { 0 };
    PlayerAttributes playerAttributes = { 0 };

    playerPosition.polygon = polygon;
    playerPosition.previousCoordinates = (Vector2) { polygon.coordinates.x, polygon.coordinates.y };
    playerPosition.coordinates = (Vector2) { polygon.coordinates.x, polygon.coordinates.y };
    playerPosition.nextCoordinates = (Vector2) { polygon.coordinates.x, polygon.coordinates.y };
    playerPosition.direction = (Vector2) { 0.0f, -1.0f };

    playerPhysics.velocity = (Vector2) { 0, 0 };
    playerPhysics.force = (Vector2) { 0.0f, 50.0f };
    playerPhysics.acceleration = (Vector2) { 0.0f, 0.0f };
    playerPhysics.deceleration = 1.25f;
    playerPhysics.mass = 1.0f;

    playerAttributes.movementSpeed = 200.0f;
    playerAttributes.jumpPower = 750.0f;
    playerAttributes.canJump = false;

    player.position = playerPosition;
    player.physics = playerPhysics;
    player.attributes = playerAttributes;

    return player;
}

void VerletIntegration(Player *player, float deltaTime)
{
    player->physics.acceleration.x = player->physics.force.x / player->physics.mass;
    player->physics.acceleration.y = player->physics.force.y / player->physics.mass;

    player->physics.velocity.x = (player->position.coordinates.x - player->position.previousCoordinates.x) / player->physics.deceleration;

    if (fabsf(player->physics.velocity.x) < 0.1)
    {
        player->physics.velocity.x = 0.0f;
    }

    player->physics.velocity.y = (player->position.coordinates.y - player->position.previousCoordinates.y);

    player->position.previousCoordinates.x = player->position.coordinates.x;
    player->position.previousCoordinates.y = player->position.coordinates.y;

    player->position.coordinates.x = (player->position.coordinates.x + player->physics.velocity.x + player->physics.acceleration.x  * deltaTime);
    player->position.coordinates.y = (player->position.coordinates.y + player->physics.velocity.y + player->physics.acceleration.y  * deltaTime);

    player->position.nextCoordinates.x = (player->position.coordinates.x + player->physics.velocity.x + player->physics.acceleration.x  * deltaTime);
    player->position.nextCoordinates.y = (player->position.coordinates.y + player->physics.velocity.y + player->physics.acceleration.y  * deltaTime);
}

void PlayerUpdate(Player *player, float deltaTime)
{
    // VerletIntegration(player, deltaTime);
    PlayerHandleInput(player, deltaTime);
    PlayerUpdateDirection(player);
    PolygonRectangleUpdateCoordinates(&player->position.polygon, player->position.coordinates.x, player->position.coordinates.y);
}

void PlayerDraw(Player *player)
{
    PolygonDrawConvex(&player->position.polygon);
    PolygonShowVerticesConvex(&player->position.polygon);
}


void PlayerUpdateDirection(Player *player)
{
    if (player->physics.velocity.x > 0.0f)
    {
        player->position.direction.x = 1.0f;
    }

    else if (player->physics.velocity.x < 0.0f)
    {
        player->position.direction.x = -1.0f;
    }

    else
    {
        player->position.direction.x = 0.0f;
    }

    if (player->physics.velocity.y > 0.0f)
    {
        player->position.direction.y = 1.0f;
    }

    else if (player->physics.velocity.y < 0.0f)
    {
        player->position.direction.y = -1.0f;
    }
    
    else
    {
        player->position.direction.y = 0.0f;
    }
}

void PlayerHandleInput(Player *player, float deltaTime)
{
    if (IsKeyDown(KEY_A))
    {
        player->position.coordinates.x -= player->attributes.movementSpeed * deltaTime; 
    }

    if (IsKeyDown(KEY_D))
    {
        player->position.coordinates.x += player->attributes.movementSpeed * deltaTime;
    }

    if (IsKeyDown(KEY_S))
    {
        player->position.coordinates.y += player->attributes.movementSpeed * deltaTime;
    }

    if (IsKeyDown(KEY_W))
    {
        player->position.coordinates.y -= player->attributes.movementSpeed * deltaTime;
    }

    if (IsKeyDown(KEY_SPACE) && player->attributes.canJump && player->position.direction.y == 0.0f)
    {
        player->position.coordinates.y -= player->attributes.jumpPower * deltaTime;
        player->attributes.canJump = false;
    }
}

