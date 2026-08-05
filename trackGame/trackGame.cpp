#include <raylib.h>
#include <iostream>
#include "car.h"

using namespace std;

int main()
{
    InitWindow(100, 100, "TrackGame");

    int monitor = GetCurrentMonitor();
    int height = GetMonitorHeight(monitor);
    int width = GetMonitorWidth(monitor);
    int vsync = GetMonitorRefreshRate(monitor);

    SetWindowSize(width, height);
    SetTargetFPS(vsync+1);
    SetWindowPosition(0, 0);
    ToggleFullscreen();

    Camera3D camera;
    camera.position = Vector3{ 10, 10, 10 };
    camera.target = Vector3{ 0, 0, 0 };
    camera.up = Vector3{ 0, 1, 0 };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    cout << monitor << '\n' << height << '\n' << width << '\n' << vsync;
    
    car aCar;
    aCar.position = Vector3{ 0, 0, 0 };

    aCar.model = LoadModel("carModelB.glb");

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        aCar.speed = 0.5 * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));

        aCar.position.x += aCar.speed;

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(50, 50);
        BeginMode3D(camera);
        DrawModel(aCar.model, aCar.position, 1.0, WHITE);
        EndMode3D();
        EndDrawing();
    }
}
