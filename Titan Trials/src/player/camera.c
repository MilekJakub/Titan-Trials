#include "camera.h"


Camera2D CameraInitialize(Player *player, int screenWidth, int screenHeight)
{
    Camera2D camera = { 0 };

    camera.target = player->position.coordinates;
    camera.offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    return camera;
}


void CameraUpdate(Camera2D *camera, Player *player, int screenWidth, int screenHeight)
{
    camera->target = player->position.coordinates;
    camera->offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}
