#include <iostream>

#include "raylib.h"
#include "raymath.h"

using namespace std;

const int win_y = 720;
const int win_x = 1000;

int main(void) {
    InitWindow(win_x, win_y, "Scan Shader2");

    Shader scanShader = LoadShader("shaders/vertex_info.vs", "shaders/scan_post2.fs");

    int camPosL = GetShaderLocation(scanShader, "camPos");
    int completionL = GetShaderLocation(scanShader, "completion");
    float completionCPU = 0.f;

    Camera cam = Camera{0};
    cam.fovy = 50;
    cam.projection = CAMERA_PERSPECTIVE;
    cam.position = Vector3{40, 20, 40};
    cam.target = Vector3{0, 0, 0};
    cam.up = Vector3{0, 1, 0};

    Model cube = LoadModelFromMesh(GenMeshSphere(5, 10, 10));
    Texture2D cubeTex = LoadTextureFromImage(GenImageChecked(4, 4, 1, 1, PURPLE, BLACK));
    cube.materials[0].shader = scanShader;
    cube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = cubeTex;
    cout << cube.materialCount << endl;

    HideCursor();
    DisableCursor();
    while (!WindowShouldClose()) {
        UpdateCamera(&cam, CAMERA_THIRD_PERSON);
        completionCPU += GetFrameTime() * 0.25f;
        if (completionCPU >= 1.f) {
            completionCPU = 0.f;
        }

        SetShaderValue(scanShader, camPosL, &cam.position, SHADER_UNIFORM_VEC3);
        SetShaderValue(scanShader, completionL, &completionCPU, SHADER_UNIFORM_FLOAT);

        BeginDrawing();
            ClearBackground(GRAY);

            BeginMode3D(cam);
                DrawModel(cube, Vector3Zero(), 1, WHITE);
            EndMode3D();
        EndDrawing();
    }

    return 0;
}