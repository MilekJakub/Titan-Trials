#ifndef RAYLIB_H
#include "raylib.h"
#endif

#ifndef CAMERA_H
#define CAMERA_H

#include "player.h"

Camera2D CameraInitialize(Player *player, int screenWidth, int screenHeight);
void     CameraUpdate(Camera2D *camera, Player *player, int screenWidth, int screenHeight);

#endif
