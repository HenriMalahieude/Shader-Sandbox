#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>

#include "raylib.h"
#include "raymath.h"

using namespace std;

int win_y = 720;
int win_x = 1000;
float yxratio = (float)win_y / (float)win_x;

int main(void) {
    srand(time(NULL));
    InitWindow(win_x, win_y, "Warp Shader");

    Shader warpShader = LoadShader(0, "shaders/warp.fs");
    warpShader.locs[SHADER_LOC_MATRIX_PROJECTION] = GetShaderLocation(warpShader, "projectionMatrix");
    warpShader.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(warpShader, "modelViewMatrix");
    SetShaderValue(warpShader, GetShaderLocation(warpShader, "yxratio"), &yxratio, SHADER_UNIFORM_FLOAT);

    Camera cam = Camera{0};
    RenderTexture2D texBuffer = LoadRenderTexture(win_x, win_y);

    Vector3 warpSource = Vector3{0, 3, 5};

    int shd_warpLocation = GetShaderLocation(warpShader, "warpLocation");
    float warpLocation[2] = {0, 0};

    int shd_warpSize = GetShaderLocation(warpShader, "warpSize");
    float warpSize = 0.01;

    int shd_warpRadius = GetShaderLocation(warpShader, "warpRadius");
    float warpRadius = 0.05;

    cam.fovy = 50;
    cam.projection = CAMERA_PERSPECTIVE;
    cam.position = Vector3{40, 20, 40};
    cam.target = Vector3{0, 0, 0};
    cam.up = Vector3{0, 1, 0};

    int tick = 0;
    HideCursor();
    DisableCursor();
    while (!WindowShouldClose()) {
        UpdateCamera(&cam, CAMERA_THIRD_PERSON);

        Vector2 sop = GetWorldToScreenEx(warpSource, cam, win_x, win_y);
        sop.x /= win_x; sop.y /= win_y; //sop.y /= yxratio;
        warpLocation[0] = sop.x;
        warpLocation[1] = 1-sop.y;
        SetShaderValue(warpShader, shd_warpLocation, warpLocation, SHADER_UNIFORM_VEC2);

        Vector3 camL = Vector3Normalize(Vector3Subtract(warpSource, cam.position));
        Vector3 camR = Vector3Normalize(Vector3CrossProduct(camL, cam.up));
        //if (Vector3Length(camR) < 0.95f) camR = Vector3CrossProduct(camL, Vector3Negate(cam.up));

        Vector3 sunT = Vector3CrossProduct(camR, camL);
        Vector3 sunB = Vector3CrossProduct(camL, camR);

        Vector3 sunR = Vector3CrossProduct(camL, sunT);
        Vector3 sunL = Vector3CrossProduct(sunT, camL);

        sunR = Vector3Scale(sunR, 2); 
        sunR = Vector3Add(sunR, warpSource);
        sunL = Vector3Add(Vector3Scale(sunL, 2), warpSource);

        sunT = Vector3Scale(sunT, 2); 
        sunT = Vector3Add(sunT, warpSource);
        sunB = Vector3Add(Vector3Scale(sunB, 2), warpSource);

        Vector2 sopSide = GetWorldToScreenEx(sunT, cam, win_x, win_y);
        if (sopSide.x < 0 || sopSide.y < 0) sopSide = GetWorldToScreenEx(sunB, cam, win_x, win_y);
        if (sopSide.x < 0 || sopSide.y < 0) sopSide = GetWorldToScreenEx(sunR, cam, win_x, win_y);
        if (sopSide.x < 0 || sopSide.y < 0) sopSide = GetWorldToScreenEx(sunL, cam, win_x, win_y);
        sopSide.x /= win_x; sopSide.y /= win_y;
        //sopSide.y /= yxratio;
        
        warpRadius = Vector2Length(Vector2Subtract(sop, sopSide)) * 1.05 * yxratio;
        SetShaderValue(warpShader, shd_warpRadius, &warpRadius, SHADER_UNIFORM_FLOAT);

        warpSize = warpRadius * 0.75;
        SetShaderValue(warpShader, shd_warpSize, &warpSize, SHADER_UNIFORM_FLOAT);

        //cout << "Loc (" << warpLocation[0] << ", " << warpLocation[1] << "), Radius: " << sizingT << " or " << sizingR << ", Size: " << warpSize << endl;

        BeginTextureMode(texBuffer);
            ClearBackground(BLACK);

            BeginMode3D(cam);
                //DrawPlane(Vector3{0, -5, 0}, Vector2{100, 100}, GRAY);
                DrawCube(Vector3{5, 5, 6}, 5, 5, 5, RED);
                DrawCubeWires(Vector3{5, 5, 6}, 5.01, 5.01, 5.01, BLACK);

                DrawSphere(Vector3Zero(), 1, GREEN);
                DrawSphereWires(Vector3Zero(), 1.01, 5, 5, BLACK);

                DrawSphere(Vector3{0, 6, -1}, 1, BROWN);
                DrawSphereWires(Vector3{0, 6, -1}, 1.01, 7, 7, BLACK);
            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginShaderMode(warpShader);
                DrawTextureRec(texBuffer.texture, Rectangle{0, 0, (float)win_x, (float)-win_y}, Vector2{0, 0}, WHITE);
            EndShaderMode();


            BeginMode3D(cam);
                DrawSphere(sunR, 0.1, GRAY);
                DrawSphere(sunT, 0.1, GRAY);
                DrawSphere(warpSource, 2, BLUE);
                DrawSphereWires(warpSource, 2.01, 10, 10, DARKBLUE);
                DrawLine3D(Vector3Zero(), Vector3{10, 0, 0}, RED);
                DrawLine3D(Vector3Zero(), Vector3{0, 10, 0}, GREEN);
                DrawLine3D(Vector3Zero(), Vector3{0, 0, 10}, BLUE);
            EndMode3D();

            //DrawRectangle((int)sopR.x, (int)sopR.y, 2, 2, DARKGREEN);
            //DrawRectangle((int)sopT.x, (int)sopT.y, 2, 2, DARKGREEN);

            DrawFPS(10, 10);
        EndDrawing();
    }

    cout << "GAME: Shutting down!";
    return 0;
}