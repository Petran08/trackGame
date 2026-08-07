#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "car.h"
#include "trackData.h"
#include "block.h"

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
    //block size 4 units
    car aCar;
    aCar.position = Vector3{ 0, 0, 0 };

    aCar.model = LoadModel("carModelB.glb");

    std::string trackName = "01.trk";

    trackData track;
    track.readTrackFile(trackName);

    std::string blockTypes[] = { "roadBlock" };

    Model blockModels[] = { LoadModel("roadBlock.glb") };

    block temp(blockTypes[0], Vector3{ 0, 0 , 0 }, 0, 0);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        aCar.updatePhysics(camera);
        
        int xMove = 4 * (IsKeyPressed(KEY_W) - IsKeyPressed(KEY_S));
        int zMove = 4 * (IsKeyPressed(KEY_D) - IsKeyPressed(KEY_A));

        temp.position.x += xMove;
        temp.position.z += zMove;

        if (IsKeyPressed(KEY_ENTER))
        {
            track.blocks.push_back(temp);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawFPS(50, 50);
        BeginMode3D(camera);
        DrawModel(aCar.model, aCar.position, 1.0, WHITE);
        DrawModel(blockModels[temp.blockID], temp.position, 1.0, WHITE);
        for (auto b : track.blocks)
        {
            DrawModel(blockModels[b.blockID], b.position, 1.0, WHITE);
        }
        EndMode3D();
        EndDrawing();
    }

    track.saveTrackFile(trackName);
}
