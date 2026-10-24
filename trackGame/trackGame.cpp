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

float circleAngle = 90;

Vector2 rec1P = Vector2{ 20, 113 }, rec1P2 = Vector2{ 20, 113 };
Vector2 rec2P = Vector2{ 335, 62 }, rec2P2 = Vector2{ 335, 62 };
Vector2 qcircP = Vector2{ 335, 111 };
Vector2 ind = Vector2{ 1450, 820 };

Color rec1c1 = BLACK, rec1c2 = BLACK, rec2c1 = BLACK, rec2c2 = BLACK, qcircc = BLACK;

float displayRpm = 1000;

float scale = 0.4f;

Texture2D quarterCircle, indicator, rec1, rec2;
Texture2D outline, inlineT;

Font russo, jadrien;

void updateRpmInd(int gear)
{
    if (displayRpm <= 1750)
    {
        rec1c1 = GREEN;
        if (displayRpm < 1250)
            rec1c1 = YELLOW;
        rec1c2 = BLACK;
        rec2c1 = BLACK;
        rec2c2 = BLACK;
        qcircc = BLACK;
        rec1P2 = rec1P + Vector2{ ((displayRpm - 500) / 1250) * rec1.width * scale, 0.0f };

    }
    if (displayRpm >= 1750 && displayRpm <= 3000)
    {
        rec1c1 = GREEN;
        rec1c2 = GREEN;
        if ((displayRpm > 2900 && gear == 7))
        {
            rec1c1 = RED;
            rec1c2 = RED;
        }
        rec2c1 = BLACK;
        rec2c2 = BLACK;
        qcircc = BLACK;
        rec1P2 = rec1P + Vector2{ ((displayRpm - 1750) / 1250) * rec1.width * scale, 0.0f };
    }
    if (displayRpm >= 3000 && displayRpm <= 3500)
    {
        rec1c1 = GREEN;
        rec1c2 = GREEN;
        rec2c1 = BLACK;
        rec2c2 = BLACK;
        qcircc = GREEN;
        circleAngle = ((3500 - displayRpm) / 500) * 90;
        rec1P2 = rec1P + Vector2{ rec1.width * scale, 0.0f };
    }
    if (displayRpm >= 3500 && displayRpm <= 4250)
    {
        rec1c1 = GREEN;
        rec1c2 = GREEN;
        rec2c1 = GREEN;
        rec2c2 = BLACK;
        qcircc = GREEN;
        if (displayRpm > 3850)
        {
            rec1c1 = RED;
            rec1c2 = RED;
            rec2c1 = RED;
            rec2c2 = BLACK;
            qcircc = RED;
        }
        rec2P2 = rec2P - Vector2{ 0.0f, ((displayRpm - 3500) / 750) * rec2.height * scale };
        rec1P2 = rec1P + Vector2{ rec1.width * scale, 0.0f };
        circleAngle = 0.0f;

    }
    if (displayRpm >= 4250)
    {
        rec1c1 = RED;
        rec1c2 = RED;
        rec2c1 = RED;
        rec2c2 = RED;
        qcircc = RED;
        rec2P2 = rec2P - Vector2{ 0.0f, ((displayRpm - 4250) / 750) * rec2.height * scale };
        rec1P2 = rec1P + Vector2{ rec1.width * scale, 0.0f };
        circleAngle = 0.0f;
    }
}

void drawRpmDisplay()
{
    DrawTextureEx(inlineT, ind, 0.0f, scale, WHITE);
    DrawTextureEx(quarterCircle, qcircP + ind, circleAngle, scale, qcircc);
    DrawTextureEx(rec1, rec1P + ind, 0.0f, scale, rec1c1);
    DrawTextureEx(rec1, rec1P2 + ind, 0.0f, scale, rec1c2);
    DrawTextureEx(rec2, rec2P + ind, 0.0f, scale, rec2c1);
    DrawTextureEx(rec2, rec2P2 + ind, 0.0f, scale, rec2c2);
    DrawTextureEx(outline, ind, 0.0f, scale, rec1c1);
    DrawTextureEx(indicator, ind, 0.0f, scale, WHITE);
}

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

    DrawText(deb.str().c_str(), 50, 100, 25, DARKGREEN);
    DrawFPS(50, 50);

    float targetRpm = 0.0f;

    if (aCar.gear > 0)
        targetRpm = aCar.rpm + 1000;
    else if(aCar.gear == 0)
        targetRpm = abs(aCar.rpm) + 1000;

    if (targetRpm < 500)
        targetRpm = 500;
    if (targetRpm > 5000)
        targetRpm = 5000;

    displayRpm += (targetRpm - displayRpm) * 0.1f;

    updateRpmInd(aCar.gear);

    drawRpmDisplay();

    stringstream gear;
    gear << aCar.gear;

    if (aCar.gear == 0)
    {
        DrawTextEx(russo, "R", Vector2{ 304, 34 } + ind, 60, 1, rec1c1);
    }
    else if (aCar.gear == 1)
    {
        DrawTextEx(russo, gear.str().c_str(), Vector2{311, 34} + ind, 60, 1, rec1c1);
    }
    else if (aCar.gear == 4)
    {
        DrawTextEx(russo, gear.str().c_str(), Vector2{ 304, 34 } + ind, 60, 1, rec1c1);
    }
    else if (aCar.gear == 6)
    {
        DrawTextEx(russo, gear.str().c_str(), Vector2{ 306, 34 } + ind, 60, 1, rec1c1);
    }
    else
    {
        DrawTextEx(russo, gear.str().c_str(), Vector2{ 308, 34 } + ind, 60, 1, rec1c1);
    }

    int drawSpeed = int(abs(aCar.displaySpeed));

    stringstream sp;

    if (drawSpeed < 10)
    {
        sp << "00" << drawSpeed;
    }
    else if (drawSpeed < 100)
    {
        sp << "0" << drawSpeed;
    }
    else
    {
        sp << drawSpeed;
    }
    if (drawSpeed < 1000)
        DrawTextEx(jadrien, sp.str().c_str(), Vector2{ 73, 12 } + ind, 110, 1, WHITE);
    else
        DrawTextEx(jadrien, sp.str().c_str(), Vector2{ 73, 12 } + ind - Vector2{ 49, 0 }, 110, 1, WHITE);

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

    russo = LoadFont("RussoOne-Regular.ttf");
    jadrien = LoadFont("JadrienDemo-vVZy.ttf");

    quarterCircle = LoadTexture("quarterCircle.png"), indicator = LoadTexture("rpm indicator.png"), rec1 = LoadTexture("rectangle1.png"), rec2 = LoadTexture("rectangle2.png");
    outline = LoadTexture("rpm indicator outline.png"), inlineT = LoadTexture("rpm indicator inline.png");


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

