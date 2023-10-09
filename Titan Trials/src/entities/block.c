#include "block.h"
#include "raylib.h"
#include "raymath.h"

Block initializeBlock(float x, float y, float width, float height, Color color, float friction, bool solid)
{
    Block block = { 0 };

    block.coordinates = (Vector2) { x, y };
    block.size = (Vector2) { width, height };
    block.hitbox = (Rectangle) { block.coordinates.x, block.coordinates.y, block.size.x, block.size.y };
    block.color = color;
    block.friction = friction;
    block.solid = solid;

    return block;
}

void updateBlocks(Block blocks[], size_t count, float deltaTime)
{
    for (int i = 0; i < count; i++)
    {
        // TODO: Implement block update logic
    }
}

void drawBlocks(Block blocks[], size_t count)
{
    for (int i = 0; i < count; i++)
    {
        DrawRectangleV(blocks[i].coordinates, blocks[i].size, blocks[i].color);
    }
}

