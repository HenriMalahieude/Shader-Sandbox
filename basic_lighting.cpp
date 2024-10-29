#include <cmath>
#include "raylib.h"

const int w_height = 720;
const int w_length = 1280;

struct Spherical {
    float radius;
    float theta;
    float phi;

    Vector3 toEuclid(){
        return Vector3{radius * cosf(theta) * sinf(phi), radius * cosf(phi), radius * sinf(theta) * cosf(phi)};
    }

    //NOTE: Size MUST be at least 3
    void toEuclidShader(float inOut[]){
        inOut[0] = radius * cosf(theta) * sinf(phi);
        inOut[1] = radius * cosf(phi);
        inOut[2] = radius * sinf(theta) * cosf(phi);
    }
};

int main() {
    InitWindow(w_length, w_height, "Shader Sandbox");

    Shader basic_lighting = LoadShader("shaders/phong/lighting.vs", "shaders/phong/lighting.fs");
    basic_lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(basic_lighting, "viewPos");
    
    float ambientAttenuation = 10.f;
    SetShaderValue(basic_lighting, GetShaderLocation(basic_lighting, "ambientAttenuation"), &ambientAttenuation, SHADER_UNIFORM_FLOAT);

    float specularExponent = 64.f;
    SetShaderValue(basic_lighting, GetShaderLocation(basic_lighting, "specularExponent"), &specularExponent, SHADER_UNIFORM_FLOAT);

    Camera3D cam = { 0 };
    cam.fovy = 45.f;
    cam.position = Vector3{15.f, 5.f, 15.f};
    cam.target = Vector3{0.f, 0.f, 0.f};
    cam.up = Vector3{0.f, 1.f, 0.f};
    cam.projection = CAMERA_PERSPECTIVE;

    Spherical sunLocation = Spherical{500.f, PI, PI/4.f};
    float sunColor[4] = {.9f, 1.f, 1.f, 1.f}; SetShaderValue(basic_lighting, GetShaderLocation(basic_lighting, "sunColor"), sunColor, SHADER_UNIFORM_VEC4);

    int sunShaderPosition = GetShaderLocation(basic_lighting, "sunLocation");
    float sunPosition[3] = { 0 }; sunLocation.toEuclidShader(sunPosition); //Making sure to initialize

    //Texture2D sphereTexture = LoadTexture("resources/thinker.png");
    Model sphere = LoadModelFromMesh(GenMeshSphere(2.f, 20, 20)); //LoadModel("resources/models/planet_n1/planet.obj");//
    sphere.materials[0].shader = basic_lighting;

    //LoadTextureFromImage(GenImageColor(64, 64, Color{50, 255, 50, 127}));
    //Model cube = LoadModelFromMesh(GenMeshCube(2, 2, 2));
    //cube.materials[0].shader = basic_lighting;
    //cube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = sphereTexture;

    while (!WindowShouldClose()){
        UpdateCamera(&cam, CAMERA_ORBITAL);
        sunLocation.toEuclidShader(sunPosition);
        SetShaderValue(basic_lighting, sunShaderPosition, sunPosition, SHADER_UNIFORM_VEC3);

        float cameraPos[3] = {cam.position.x, cam.position.y, cam.position.z};
        SetShaderValue(basic_lighting, basic_lighting.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(cam);
                DrawGrid(2, 5000.f);

                DrawModel(sphere, Vector3{0, 0, 0.f}, 1.f, WHITE);
                //DrawModel(cube, Vector3{0, 0, -5.f}, 1.f, WHITE);
                //DrawSphereWires(Vector3{0, 0, 0}, 4.5f, 10, 10, Color{50, 255, 50, 127});

            EndMode3D();
        EndDrawing();
    }

    return 0;
}
