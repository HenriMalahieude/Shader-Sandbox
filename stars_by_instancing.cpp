#include <ctime>
#include <cstdio>
#include "raylib.h"
#include "raymath.h"

const int MAX_STARS = 1000;
const int MAX_COLORS = 20; //look in star_instancing.vs to make sure this matches

int star_color_locations[MAX_COLORS];

int lation_size = 0;
int lation_size_pos;

void Star_Shader_Get_Pointers(Shader shd) {
	for (int i = 0; i < MAX_COLORS; i++) {
		star_color_locations[i] = GetShaderLocation(shd, TextFormat("star_colors[%i]", i));
	}

	lation_size_pos = GetShaderLocation(shd, "constellation_size");
}

void Star_Shader_Color_Randomize(Shader shd) {
	for (int i = 0; i < MAX_COLORS; i++) {
		float col[4] = {(GetRandomValue(0, 255) / 255.f), (GetRandomValue(100, 255) / 255.f), (GetRandomValue(0, 255) / 255.f), 1.f};
		int mr = GetRandomValue(1, 2);
		if (mr == 1) {
			col[0] = 1.f;
		}else if (mr == 2) {
			col[2] = 1.f;
		}

		SetShaderValue(shd, star_color_locations[i], &col, SHADER_UNIFORM_VEC4);
	}
}

void Star_Shader_Color_Random_Black(Shader shd) {
	int index = GetRandomValue(0, MAX_COLORS-1);

	float col[4] = {0, 0, 0, 0};
	SetShaderValue(shd, star_color_locations[index], &col, SHADER_UNIFORM_VEC4);
}

void Star_Shader_Locations_Randomize(Matrix *trnsfrms, Shader shd) {
	lation_size = 0;
	SetShaderValue(shd, lation_size_pos, &lation_size, SHADER_UNIFORM_INT);

	for (int i = 0; i < MAX_STARS; i++) {
		const float prec = 100000000.f;
		float theta = GetRandomValue(0, (int)(prec * 2*PI)) / prec;
		float phi = GetRandomValue(0, (int)(prec * 2*PI)) / prec;
		
		const float radius = GetRandomValue(500, 700);
		float x = radius * cosf(phi) * cosf(theta);
		float y = radius * sinf(phi);
		float z = radius * cosf(phi) * sinf(theta);

		if (fabs(y) >= (radius * 0.8f)){
			theta = GetRandomValue(0, (int)(prec * 2*PI)) / prec;
			phi = GetRandomValue(0, (int)(prec * 2*PI)) / prec;

			x = radius * cosf(phi) * cosf(theta);
			y = radius * sinf(phi);
			z = radius * cosf(phi) * sinf(theta);
		}

		trnsfrms[i] = MatrixTranslate(x, y, z);
	}
}

void Star_Shader_Constellation_Generate(Matrix *trnsfrms, Shader shd, int total){
	const float precision = 100000.f;
    const int maxMovement = (PI / 4.f)*precision;

    float theta = GetRandomValue(0, 6283) / 1000.f; //0 - 2 PI
    float phi = GetRandomValue(0, 6283) / 1000.f; // 0 - 2 PI

	//The way it's formatted, the first star in the list is at the center of the constellation
    for (int i = 0; i < total; i++){
		float movement1 = (GetRandomValue(0, maxMovement*2) - maxMovement) / precision;
		float movement2 = (GetRandomValue(0, maxMovement*2) - maxMovement) / precision;
        float nThe = theta + (i/((float)total) - 1.f) * movement1;
        float nPhi = phi   + (i/((float)total) - 1.f) * movement2;

		const float radius = 350.f;
		const float x = radius * cosf(nPhi) * cosf(nThe);
		const float y = radius * sinf(nPhi);
		const float z = radius * cosf(nPhi) * sinf(nThe);

		trnsfrms[i] = MatrixTranslate(x, y, z);
	}

	lation_size = total;
	SetShaderValue(shd, lation_size_pos, &lation_size, SHADER_UNIFORM_INT);
}

int main(void) {
	SetRandomSeed(time(NULL));
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Stars by instancing then bloom");

	Camera camera;
	camera.position = Vector3{30.f, 30.f, 10.f};
	camera.target = Vector3Zero();
	camera.up = Vector3{0.f, 0.f, 1.f};
	camera.fovy = 45.f;
	camera.projection = CAMERA_PERSPECTIVE;

	Mesh sphere = GenMeshSphere(1.5f, 15, 15);
	Matrix *star_transforms = (Matrix *)RL_CALLOC(MAX_STARS, sizeof(Matrix));

	Shader instancing_shd = LoadShader("shaders/star_instancing.vs", "shaders/straight_through_color.fs");
	Star_Shader_Get_Pointers(instancing_shd);
	Star_Shader_Locations_Randomize(star_transforms, instancing_shd);
	instancing_shd.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(instancing_shd, "mvp");
	instancing_shd.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(instancing_shd, "instanceTransform");

	Material matInstances = LoadMaterialDefault();
    matInstances.shader = instancing_shd;
    matInstances.maps[MATERIAL_MAP_DIFFUSE].color = RED;

	RenderTexture2D star_holder = LoadRenderTexture(screenWidth, screenHeight);
	Shader bloom_shd = LoadShader(0, "shaders/bloom.fs");

	Star_Shader_Color_Randomize(instancing_shd);
	DisableCursor();
	SetTargetFPS(60);
	while (!WindowShouldClose()) {
		UpdateCamera(&camera, CAMERA_THIRD_PERSON);
		//camera.up = Vector3{0.f, 1.f, 0.f};

		if (IsKeyPressed(KEY_Z)) {
			Star_Shader_Color_Randomize(instancing_shd);
		}else if (IsKeyPressed(KEY_X)) {
			Star_Shader_Locations_Randomize(star_transforms, instancing_shd);
		}else if (IsKeyPressed(KEY_C)) {
			Star_Shader_Color_Random_Black(instancing_shd);
		}else if (IsKeyPressed(KEY_V)) {
			Star_Shader_Constellation_Generate(star_transforms, instancing_shd, 50);
		}

		BeginTextureMode(star_holder);
			ClearBackground(BLACK);

			BeginMode3D(camera);
				DrawMeshInstanced(sphere, matInstances, star_transforms, MAX_STARS);
			EndMode3D();
		EndTextureMode();

		BeginDrawing();
			ClearBackground(BLACK);

			BeginShaderMode(bloom_shd);
				DrawTextureRec(star_holder.texture, Rectangle{0, 0, (float)star_holder.texture.width, (float)-star_holder.texture.height}, Vector2Zero(), WHITE);
			EndShaderMode();


			BeginMode3D(camera);
				//DrawCube(Vector3Zero(), 5, 5, 5, RED);
				DrawCubeWires(Vector3Zero(), 5, 5, 5, RED);
			EndMode3D();
			DrawFPS(0, 0);
		EndDrawing();
	}

	return 0;
}