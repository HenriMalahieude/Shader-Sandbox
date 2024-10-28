#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <iostream>

using namespace std;

const int wl = 900;
const int wh = wl * 9 / 16;

int main() {
	InitWindow(wl, wh, "Cam Glitch 2");

	Texture2D blank = LoadTextureFromImage(GenImageColor(wl, wh, Color{0, 0, 0, 0}));
	Shader glitch = LoadShader(0, "./shaders/glitch.fs");

	int dcl = GetShaderLocation(glitch, "dead_chance");
	float cpu_deadChance = 0.01f;

	int rcl = GetShaderLocation(glitch, "rad_chance");
	float cpu_radChance = 0.05f;

	int tl = GetShaderLocation(glitch, "time");
	float cpu_time = 0.f;

	Camera camera;
	camera.position = Vector3{30.f, 30.f, 10.f};
	camera.target = Vector3Zero();
	camera.up = Vector3{0.f, 0.f, 1.f};
	camera.fovy = 45.f;
	camera.projection = CAMERA_PERSPECTIVE;

	float local_time = 0;
	float update_time = 0.75;
	while (!WindowShouldClose()) {
		UpdateCamera(&camera, CAMERA_ORBITAL);

		local_time += GetFrameTime();
		if (local_time > update_time) {
			local_time = 0;

			cpu_time += 1;
			if (cpu_time > 100) cpu_time = 0;
			SetShaderValue(glitch, tl, &cpu_time, SHADER_UNIFORM_FLOAT);
		}

		int pressed = GetKeyPressed();

		/*if (pressed == KEY_Z || pressed == KEY_C) {
			if (pressed == KEY_Z) update_time += 0.01f;
			if (pressed == KEY_C) update_time -= 0.01f;
			update_time = max(update_time, 0.1f);
			cout << "Update Time: " << update_time << endl;
		} */

		if (pressed == KEY_Q || pressed == KEY_E) {
			if (pressed == KEY_Q) cpu_deadChance += 0.01f;
			if (pressed == KEY_E) cpu_deadChance -= 0.01f;
			cpu_deadChance = max(cpu_deadChance, 0.f);

			cout << "Dead Chance: " << cpu_deadChance << endl;

			SetShaderValue(glitch, dcl, &cpu_deadChance, SHADER_UNIFORM_FLOAT);
		}

		if (pressed == KEY_A || pressed == KEY_D) {
			if (pressed == KEY_A) cpu_radChance += 0.01f;
			if (pressed == KEY_D) cpu_radChance -= 0.01f;
			cpu_radChance = max(cpu_radChance, 0.f);
			update_time = 0.75f * (1 - (cpu_radChance / 0.2f)); update_time = max(update_time, 0.1f);
			cout << "Rad Chance: " << cpu_radChance << " Update Time: " << update_time << endl;

			SetShaderValue(glitch, rcl, &cpu_radChance, SHADER_UNIFORM_FLOAT);
		}

		BeginDrawing();
			ClearBackground(GRAY);

			BeginMode3D(camera);

				DrawSphereWires(Vector3Zero(), 5, 10, 10, RED);

				DrawCube(Vector3{15, 5, 1}, 4, 4, 4, GREEN);
				DrawCubeWires(Vector3{15, 5, 1}, 4.1, 4.1, 4.1, WHITE);

			EndMode3D();

			BeginShaderMode(glitch);
				DrawTexture(blank, 0, 0, WHITE);
			EndShaderMode();

			DrawFPS(0, 0);
		EndDrawing();
	}

	return 0;
}