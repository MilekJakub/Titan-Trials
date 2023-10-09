#include "raylib.h"
#include "player.h"
#include "block.h"

// Global variables

int screenWidth = 1024;
int screenHeight = 768;
Player player;
Block blocks[4];

void initializeGame();
void drawDebugInfo();

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Titan Trials");
    SetTargetFPS(60);

    initializeGame();

    while (!WindowShouldClose())
    {
        ClearBackground(BLACK);
        float deltaTime = GetFrameTime();

        updateBlocks(blocks, 4, deltaTime);
        updatePlayer(&player, blocks, 4, deltaTime);

        BeginDrawing();
            drawBlocks(blocks, 4);
            drawPlayer(player);
            drawDebugInfo();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void initializeGame()
{
    printf("INFO: Initializing game resources.\n");

    player = initializePlayer(screenWidth / 2.0f, screenHeight / 2.0f, 32.0f, 64.0f);

    Block floor = initializeBlock(0, screenHeight - 10.0f, screenWidth * 2.0f, 10.0f, RAYWHITE, 1.0f, true);
    Block platform1 = initializeBlock(screenWidth / 2.0f - 160.0f, screenHeight - 74.0f, 64.0f, 64.0f, BLUE, 1.0f, true);
    Block platform2 = initializeBlock(screenWidth / 2.0f - 16.0f, screenHeight - 74.0f, 64.0f, 64.0f, BLUE, 1.0f, true);
    Block platform3 = initializeBlock(screenWidth / 2.0f + 128.0f, screenHeight - 74.0f, 64.0f, 64.0f, BLUE, 1.0f, true);
    blocks[0] = floor;
    blocks[1] = platform1;
    blocks[2] = platform2;
    blocks[3] = platform3;
}

void drawDebugInfo()
{
    DrawFPS(0, 0);
    DrawText(TextFormat("coordinates.x: %f", player.position.coordinates.x), 0, 40, 20, WHITE);
    DrawText(TextFormat("coordinates.y: %f", player.position.coordinates.y), 0, 60, 20, WHITE);

    DrawText(TextFormat("velocity.x: %f", player.physics.velocity.x), 0, 80, 20, WHITE);
    DrawText(TextFormat("velocity.y: %f", player.physics.velocity.y), 0, 100, 20, WHITE);

    DrawText(TextFormat("direction.x: %f", player.position.direction.x), 0, 140, 20, WHITE);
    DrawText(TextFormat("direction.y: %f", player.position.direction.y), 0, 160, 20, WHITE);
}

