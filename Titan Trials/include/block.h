#pragma once

#include "raylib.h"

typedef struct Block {
    Vector2 coordinates;
	Vector2 size;
    Rectangle hitbox;
    Color color;
    float friction;
    bool solid;
} Block;

Block initializeBlock(float x, float y, float width, float height, Color color, float friction, bool solid);
void updateBlocks(Block blocks[], size_t count, float deltaTime);
void drawBlocks(Block blocks[], size_t count);

