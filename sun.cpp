#include <ctime>
#include <iostream>
#include "raylib.h"
#include "raymath.h"

using namespace std;

const int w_length = 800, w_height = 450;

int main(){
	SetRandomSeed(time(NULL));

	InitWindow(w_length, w_length, "Sun Lighting");

	Camera camera;
	camera.position = Vector3{10.f, 5.f, 0.f};
	camera.target = Vector3Zero();
	camera.up = Vector3{0.f, 1.f, 0.f};
	camera.fovy = 45.f;
	camera.projection = CAMERA_PERSPECTIVE;

	Model planet = LoadModel("../../project-satellite/_build/resources/models/planet_n8/planet.obj");
	Model moon = LoadModel("../../project-satellite/_build/resources/models/moon_n3/bloodmoon.obj");

	Shader shd = LoadShader("shaders/vertex_info.vs", "shaders/lighting.fs");
	int sunp_pos, sunc_pos, specc_pos, ambi_pos;
	sunp_pos = GetShaderLocation(shd, "sun_pos");
	sunc_pos = GetShaderLocation(shd, "sun_color");
	specc_pos = GetShaderLocation(shd, "specular_coef");
	ambi_pos = GetShaderLocation(shd, "ambient_intensity");
	shd.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shd, "viewPos");

	planet.materials[0].shader = shd;
	moon.materials[0].shader = shd;

	float sunrad = 0.f; //Set in loop
	float spec_coef = 0.f; SetShaderValue(shd, specc_pos, &spec_coef, SHADER_UNIFORM_FLOAT);
	float ambient_intensity = 0.0001f; SetShaderValue(shd, ambi_pos, &ambient_intensity, SHADER_UNIFORM_FLOAT);
	float sun_color[4] = {1.0f, 1.0f, 0.5f, 1.0f}; SetShaderValue(shd, sunc_pos, sun_color, SHADER_UNIFORM_VEC4);

	SetTargetFPS(60);
	while(!WindowShouldClose()) {
		UpdateCamera(&camera, CAMERA_ORBITAL);
		sunrad += GetFrameTime() * (PI/3.f);
		if (sunrad >= 2*PI) {
			sunrad = 0.f;
		}
		
		float camPos[3] = {camera.position.x, camera.position.y, camera.position.z};
		SetShaderValue(shd, shd.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);

		float sun_pos[3] = {300*cosf(sunrad), 2.f, 300*sinf(sunrad)};
		SetShaderValue(shd, sunp_pos, sun_pos, SHADER_UNIFORM_VEC3);

		BeginDrawing();
			ClearBackground(BLACK);

			BeginMode3D(camera);
				DrawModel(planet, Vector3Zero(), 0.75f, WHITE);
				DrawModel(moon, Vector3{0.f, 3.f, 2.f}, 0.5f, WHITE);
			EndMode3D();
			DrawFPS(10, 10);
		EndDrawing();
	}
}