#include "raylib.h"

const int w_l = 1280;
const int w_h = 720;

void ReloadShader(Model &sun, Shader &sun_shader){
    UnloadShader(sun_shader);
    sun_shader = LoadShader("shaders/sun/sun.vs", "shaders/sun/sun.fs");

    sun.materials[0].shader = sun_shader;
}

int main(){
    InitWindow(w_l, w_h, "Sun Shader");

    Shader sun_shader = LoadShader("shaders/sun/sun.vs", "shaders/sun/sun.fs");

    Model sun = LoadModelFromMesh(GenMeshSphere(3.f, 15, 15));//LoadModel("resources/models/planet_n1/normal_planet.fbx");
    sun.materials[0].shader = sun_shader;
    sun.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = LoadTextureFromImage(GenImageColor(64, 64, GREEN));

    Camera3D camera;
    camera.fovy = 50.f;
    camera.position = Vector3{15.f, 5.f, 15.f};
    camera.projection = CAMERA_PERSPECTIVE;
    camera.target = Vector3{0.f, 0.f, 0.f};
    camera.up = Vector3{0.f, 1.f, 0.f};

    float reload_deb = 0.f;

    while(!WindowShouldClose()){
        UpdateCamera(&camera, CAMERA_ORBITAL);

        if (reload_deb > 0.f){
            reload_deb -= GetFrameTime();
        }else{
            reload_deb = 0.f;
            if (GetKeyPressed() == KEY_R){
                reload_deb = 2.f;
                ReloadShader(sun, sun_shader);
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawText("Press [R] to reload shader", 0, 15, 15, WHITE);

            BeginMode3D(camera);
                DrawGrid(4, 100.f);
                DrawModel(sun, Vector3{0.f, 0.f, 0.f}, 1.f, WHITE);
                //DrawSphereWires(Vector3{0.f, 0.f, 0.f}, 3.1f, 20, 20, Color{10, 255, 10, 127});

            EndMode3D();
        EndDrawing();
    }

    UnloadShader(sun_shader);

    return 0;
}