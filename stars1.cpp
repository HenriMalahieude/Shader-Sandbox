#include "raylib.h"
#include "raymath.h"
#include <ctime>
#include <vector>

using namespace std;

const int MAX_STARS = 1000;

struct Shader_Sphere{
	float theta = 0.f;
	float phi = 0.f;
	int enabled = 0;
	Color col;

	int theta_pos;
	int phi_pos;
	int enabled_pos;
	int col_pos;
};

Shader_Sphere vec3ToSphere(Vector3 coord) {
	//float radius = Vector3Distance(Vector3Zero(), coord);
	float theta = atan2f(coord.z, coord.x);
	float phi = atan2f(sqrt(coord.x * coord.x + coord.z * coord.z), coord.y);
	return Shader_Sphere{theta, phi};
}

void updateShaderStar(Shader_Sphere sph) {

}

Shader_Sphere createRandomShaderStar(float theta, float phi, int enabled, Shader shd, int star_index) {
	Shader_Sphere sph;
	sph.phi = phi;
	sph.theta = theta;
	sph.enabled = enabled;

	sph.phi_pos = GetShaderLocation(shd, TextFormat("star_locations[%i].phi", star_index));
	sph.theta_pos = GetShaderLocation(shd, TextFormat("star_location[%i].theta", star_index));
	sph.enabled_pos = GetShaderLocation(shd, TextFormat("star_location[%i].enabled", star_index));
	sph.col_pos = GetShaderLocation(shd, TextFormat("star_location[%i].col", star_index));

	updateShaderStar(sph);
	return sph;
}

int main() {
	const int screenWidth = 800;
	const int screenHeight = 450;
	SetRandomSeed(time(NULL));

	InitWindow(screenWidth, screenHeight, "Stars Shader Playground");

	Camera cam = { 0 };
	cam.position = Vector3{2.f, 4.f, 6.f};
	cam.target = Vector3{0.f, 0.f, 0.f};
	cam.up = Vector3{0.f, 1.f, 0.f};
	cam.fovy = 45.f;
	cam.projection = CAMERA_PERSPECTIVE;

	Shader star_shd = LoadShader(0, "shaders/stars.fs");

	Shader_Sphere stars[MAX_STARS];
	for (int i = 0; i < MAX_STARS; i++){
		stars[i] = createRandomShaderStar(GetRandomValue(0, 628) / 100.f, GetRandomValue(0, 628) / 100.f, 1, star_shd, i);
	}

	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		UpdateCamera(&cam, CAMERA_ORBITAL);

	}

	return 0;
}