#include "raylib.h"
#include "player.h"
#include "camera.h"

int screenWidth = 1024;
int screenHeight = 768;
Camera2D camera;
Player player;
ConvexPolygon convexPolygons[4];
ConcavePolygon concavePolygons[1];

void InitializeGame();
void DrawDebugInfo();

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Titan Trials");
    SetTargetFPS(60);

    InitializeGame();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        PlayerUpdate(&player, deltaTime);
        CameraUpdate(&camera, &player, screenWidth, screenHeight);
        
        for (size_t i = 0; i < 2; i++)
        {
            PolygonRotateConvex(&convexPolygons[i], 0.05f);
            PolygonHandleCollisionConvexConvex(&player.position.polygon, &convexPolygons[i]);
        }

        for (size_t i = 0; i < 1; i++)
        {
            PolygonRotateConcave(&concavePolygons[i], 0.01f);
            PolygonHandleCollisionConcaveConvex(&concavePolygons[i], &player.position.polygon);
        }

        BeginDrawing();
            ClearBackground(BLACK);
        
            BeginMode2D(camera);
                PolygonDrawArrayConvex(convexPolygons, 2);
                PolygonDrawArrayConcave(concavePolygons, 1);
                PlayerDraw(&player);
            EndMode2D();

            DrawDebugInfo();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void InitializeGame()
{
    printf("INFO: Initializing game resources.\n");

    player = PlayerInitialize(PolygonInitializeRectangle(0, 0, 32.0f, 32.0f, RED));
    camera = CameraInitialize(&player, screenWidth, screenHeight);

    ConvexPolygon square = PolygonInitializeRectangle(400.0f, 25.0f, 64.0f, 64.0f, BLUE);
    ConvexPolygon triangle = PolygonInitializeTriangle((Vector2) { 100.0f, 250.0f }, (Vector2) { 220.0f, 250.0f }, (Vector2) { 160.0f, 370.0f }, BLUE);

    convexPolygons[0] = square;
    convexPolygons[1] = triangle;

    ConvexPolygon polygons[] =
    {
        PolygonInitializeRectangle(0, 0, 100, 100, BLUE),
        PolygonInitializeRectangle(100, 100, 100, 100, BLUE),
        PolygonInitializeRectangle(0, 200, 100, 100, BLUE),
        PolygonInitializeRectangle(-100, 100, 100, 100, BLUE),
        PolygonInitializeRectangle(0, 100, 100, 100, BLUE),
    };

    ConcavePolygon plus2 = PolygonInitializeConcave(polygons, 5, BLUE);
    concavePolygons[0] = plus2;
}

void DrawDebugInfo()
{
    DrawFPS(0, 0);
    DrawText(TextFormat("coordinates.x: %f", player.position.coordinates.x), 0, 40, 20, WHITE);
    DrawText(TextFormat("coordinates.y: %f", player.position.coordinates.y), 0, 60, 20, WHITE);
}

