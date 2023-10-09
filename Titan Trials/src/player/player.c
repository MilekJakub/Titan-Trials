#include "raylib.h"
#include "raymath.h"
#include <math.h>

#include "player.h"
#include "block.h"

Player initializePlayer(float x, float y, float width, float height)
{
    Player player = { 0 };
    PlayerPosition playerPosition = { 0 };
    PlayerPhysics playerPhysics = { 0 };
    PlayerAttributes playerAttributes = { 0 };

    playerPosition.previousCoordinates = (Vector2) { x, y };
    playerPosition.coordinates = (Vector2) { x, y };
    playerPosition.nextCoordinates = (Vector2) { x, y };
    playerPosition.size = (Vector2) { width, height };
    playerPosition.direction = (Vector2) { 0.0f, -1.0f };
    playerPosition.hitbox = (Rectangle) { x, y, width, height };

    playerPhysics.velocity = (Vector2) { 0, 0 };
    playerPhysics.force = (Vector2) { 0.0f, 50.0f };
    playerPhysics.acceleration = (Vector2) { 0.0f, 0.0f };
    playerPhysics.deceleration = 1.25f;
    playerPhysics.mass = 1.0f;

    playerAttributes.movementSpeed = 75.0f;
    playerAttributes.jumpPower = 600.0f;
    playerAttributes.canJump = false;
    playerAttributes.color = RED;

    player.position = playerPosition;
    player.physics = playerPhysics;
    player.attributes = playerAttributes;

    return player;
}

void verletIntegration(Player *player, float deltaTime)
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

    player->position.hitbox.x = player->position.coordinates.x;
    player->position.hitbox.y = player->position.coordinates.y;
}

void updatePlayer(Player *player, Block blocks[], size_t block_count, float deltaTime)
{
    verletIntegration(player, deltaTime);

    for (int i = 0; i < block_count; i++)
    {
        handleBlockCollision(player, blocks[i]);
    }

    handlePlayerInput(player, deltaTime);
    updateDirection(player);
}

void drawPlayer(Player player)
{
    // DrawRectangle(player.position.previousCoordinates.x, player.position.previousCoordinates.y, player.position.size.x, player.position.size.y, (Color){ 0, 228, 48, 100 });
    DrawRectangle(player.position.coordinates.x, player.position.coordinates.y, player.position.size.x, player.position.size.y, player.attributes.color);
    // DrawRectangle(player.position.nextCoordinates.x, player.position.nextCoordinates.y, player.position.size.x, player.position.size.y, (Color){ 0, 121, 241, 100 });
}

void handleBlockCollision(Player *player, Block block)
{
    if (CheckCollisionRecs(player->position.hitbox, block.hitbox))
    {
        Rectangle collisionRectangle = GetCollisionRec(player->position.hitbox, block.hitbox);

        // Colliding from the top
        if (player->position.coordinates.y - player->position.size.y <= block.coordinates.y)
        {
            if (collisionRectangle.y > player->position.coordinates.y && collisionRectangle.width > collisionRectangle.height)
            {
                DrawText("Colliding from the top", 500, 440, 20, GREEN);
                player->position.previousCoordinates.y = block.coordinates.y - player->position.size.y;
                player->position.coordinates.y = block.coordinates.y - player->position.size.y;
                player->position.nextCoordinates.y = block.coordinates.y - player->position.size.y;
                player->physics.velocity.y = 0.0f;
                player->attributes.canJump = true;
            }
        }

        // Colliding from the bottom
        if (player->position.coordinates.y >= block.coordinates.y - block.size.y)
        {
            if (collisionRectangle.y < player->position.coordinates.y + player->position.size.y && collisionRectangle.width > collisionRectangle.height)
            {
                DrawText("Colliding from the bottom", 500, 460, 20, GREEN);
                player->position.previousCoordinates.y = block.coordinates.y + block.size.y;
                player->position.coordinates.y = block.coordinates.y + block.size.y;
                player->position.nextCoordinates.y = block.coordinates.y + block.size.y;
                player->physics.velocity.y = 0.0f;
            }
        }

        // Colliding from the left
        if (player->position.coordinates.x + player->position.size.x >= block.coordinates.x)
        {
            if (player->position.coordinates.x < collisionRectangle.x && collisionRectangle.height > collisionRectangle.width)
            {
                DrawText("Colliding from the left", 500, 480, 20, GREEN);
                player->position.previousCoordinates.x = block.coordinates.x - player->position.size.x;
                player->position.coordinates.x = block.coordinates.x - player->position.size.x;
                player->position.nextCoordinates.x = block.coordinates.x - player->position.size.x;
            }
        }

        // Colliding from the right
        if (player->position.coordinates.x <= block.coordinates.x + block.size.x)
        {
            if (player->position.coordinates.x + player->position.size.x > collisionRectangle.x + collisionRectangle.width && collisionRectangle.height > collisionRectangle.width)
            {
                DrawText("Colliding from the right", 500, 500, 20, GREEN);
                player->position.previousCoordinates.x = block.coordinates.x + block.size.x;
                player->position.coordinates.x = block.coordinates.x + block.size.x;
                player->position.nextCoordinates.x = block.coordinates.x + block.size.x;
            }
        }
    }
}

void updateDirection(Player *player)
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

void handlePlayerInput(Player *player, float deltaTime)
{
    if (IsKeyDown(KEY_A))
    {
        player->position.coordinates.x -= player->attributes.movementSpeed * deltaTime; 
    }

    if (IsKeyDown(KEY_D))
    {
        player->position.coordinates.x += player->attributes.movementSpeed * deltaTime;
    }

    if (IsKeyDown(KEY_SPACE) && player->attributes.canJump && player->position.direction.y == 0.0f)
    {
        player->position.coordinates.y -= player->attributes.jumpPower * deltaTime;
        player->attributes.canJump = false;
    }
}

