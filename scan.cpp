#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>

#include "raylib.h"
#include "raymath.h"

using namespace std;

const int win_y = 720;
const int win_x = 1000;

int main(void) {
    InitWindow(win_x, win_y, "Scan Shader");

    Shader scanShader = LoadShader(0, "shaders/scan_post.fs");
    
    int scan_pos_l = GetShaderLocation(scanShader, "position");
    float scanPosCPU = 0.5f;

    Camera cam = Camera{0};
    cam.fovy = 50;
    cam.projection = CAMERA_PERSPECTIVE;
    cam.position = Vector3{40, 20, 40};
    cam.target = Vector3{0, 0, 0};
    cam.up = Vector3{0, 1, 0};

    RenderTexture2D texBuffer = LoadRenderTexture(win_x, win_y);

    while (!WindowShouldClose()) {
        UpdateCamera(&cam, CAMERA_THIRD_PERSON);

        if (IsKeyDown(KEY_F)) {
            scanPosCPU -= 0.5 * GetFrameTime();
        } else if (IsKeyDown(KEY_G)) {
            scanPosCPU += 0.5 * GetFrameTime();
        }
        if (scanPosCPU > 1.1f) scanPosCPU = -0.1f;
        if (scanPosCPU < -0.1f) scanPosCPU = 1.1f;

        SetShaderValue(scanShader, scan_pos_l, &scanPosCPU, SHADER_UNIFORM_FLOAT);
        cout << scanPosCPU << endl;

        BeginTextureMode(texBuffer);
            ClearBackground(BLACK);

            BeginMode3D(cam);
                DrawCube(Vector3{5, 5, 6}, 5, 5, 5, RED);
                DrawCubeWires(Vector3{5, 5, 6}, 5.05, 5.05, 5.05, BLUE);

                DrawSphere(Vector3Zero(), 1, GREEN);
                DrawSphereWires(Vector3Zero(), 1.05, 10, 10, BLACK);
            EndMode3D();
        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);

            BeginShaderMode(scanShader);
                DrawTextureRec(texBuffer.texture, Rectangle{0, 0, (float)win_x, (float)-win_y}, Vector2{0, 0}, WHITE);
            EndShaderMode();
            DrawFPS(0, 0);
        EndDrawing();
    }

    cout << "GAME: Shutting down!\n";
    return 0;
}