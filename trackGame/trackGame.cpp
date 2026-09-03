#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <sstream>
#include "car.h"
#include "trackData.h"
#include "block.h"

using namespace std;

std::string blockTypes[] = { "roadBlock", "roadBlockS"};

Model blockModels[10];

void drawScreen(Camera3D camera, car aCar, trackData track, block temp)
{
    stringstream deb;

    BeginDrawing();
    ClearBackground(SKYBLUE);
    BeginMode3D(camera);


    Matrix carRotation = {
        aCar.forward.x, aCar.up.x,    aCar.right.x,  0,
        aCar.forward.y, aCar.up.y,    aCar.right.y,  0,
        aCar.forward.z, aCar.up.z,    aCar.right.z,  0,
        0,              0,            0,              1
    };

    aCar.model.transform = carRotation;

    for(auto s : aCar.collisionSpheres)
    {
        DrawSphere(s.center, s.radius, GREEN);
    }
    
    DrawModel(aCar.model, aCar.position, 1.0f, WHITE);

    DrawModel(blockModels[temp.blockID], Vector3Add(temp.position, Vector3{ 0.0f, 0.01f, 0.0f }), 1.0f, Color{ 255, 255, 255, 100 });

    for (auto b : track.blocks)
    {
        DrawModel(blockModels[b.blockID], b.position, 1.0f, WHITE);
    }

    EndMode3D();

    deb << "Angle: " << aCar.angle.x << " " << aCar.angle.y << " " << aCar.angle.z;
    deb << "\noldAngle: " << aCar.oldAngle.x << " " << aCar.oldAngle.y << " " << aCar.oldAngle.z;
    deb << "\nAngleMovement: " << aCar.angleMovement.x << " " << aCar.angleMovement.y << " " << aCar.angleMovement.z;
    deb << "\nposition: " << aCar.position.x << " " << aCar.position.y << " " << aCar.position.z;
    deb << "\nUp: " << aCar.up.x << ' ' << aCar.up.y << ' ' << aCar.up.z;
    deb << "\nright: " << aCar.right.x << ' ' << aCar.right.y << ' ' << aCar.right.z;
    deb << "\nforward: " << aCar.forward.x << ' ' << aCar.forward.y << ' ' << aCar.forward.z;
    deb << "\n\nSpeed: " << aCar.speed;
    deb << "\ndisplaySpeed: " << aCar.displaySpeed;
    deb << "\ngear: " << aCar.gear;
    deb << "\nrpm: " << aCar.rpm;
    deb << "\nautomatic: " << int(aCar.automatic);

    DrawText(deb.str().c_str(), 50, 100, 25, DARKGREEN);
    DrawFPS(50, 50);

    EndDrawing();
}

int main()
{
    InitWindow(100, 100, "TrackGame");

    int monitor = GetCurrentMonitor();
    int height = GetMonitorHeight(monitor);
    int width = GetMonitorWidth(monitor);
    int vsync = GetMonitorRefreshRate(monitor);

    SetWindowSize(width, height);
    //SetTargetFPS(vsync);
    SetTargetFPS(100);
    //SetTargetFPS(5);
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

    blockModels[0] = LoadModel("roadBlock.glb");
    blockModels[1] = LoadModel("roadBlockS.glb");


    aCar.model = LoadModel("carModelB.glb");

    std::string trackName = "01.trk";

    trackData track;
    track.readTrackFile(trackName, blockModels);

    int blockId = 1;

    block temp(blockTypes[blockId], Vector3{ 0, 0 , 0 }, 0, blockId, blockModels[blockId]);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }

        aCar.updatePhysics(camera, track);
        
        int xMove = 20 * (IsKeyPressed(KEY_W) - IsKeyPressed(KEY_S));
        int zMove = 20 * (IsKeyPressed(KEY_D) - IsKeyPressed(KEY_A));

        temp.position.x += xMove;
        temp.position.z += zMove;

        if (IsKeyPressed(KEY_ENTER))
        {
            track.blocks.push_back(temp);
        }

        drawScreen(camera, aCar, track, temp);

    }

    track.saveTrackFile(trackName);
}
