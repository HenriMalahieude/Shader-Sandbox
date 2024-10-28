#include <vector>
#include <iostream>
#include "raylib.h"
#include "raymath.h"

using namespace std;

const int wlength = 800, wheight = 450;

Texture2D flareTxr;
float downsample = 10.f;

void RenderFlare(Vector2 loc, float sizing, Color col, float brightness) {
	DrawTexturePro(
			flareTxr,
			Rectangle{0, 0, (float)flareTxr.width, (float)flareTxr.height}, 
			Rectangle{loc.x, loc.y, sizing, sizing}, 
			Vector2{sizing/2.f, sizing/2.f},
			0.f, 
			ColorAlpha(col, brightness)
		);
}

void LensFlareRender(Camera cam, Vector3 sunPos, float sunSize, int total) {
	total += (1 - (total % 2)); //bad habit huh, whatever; keeping it odd because we don't want anything exactly in the middle
	const float bright_offset = 0.2f;


	float planetSize = 15.f;
	Vector3 lookDir = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	Vector3 sunDir = Vector3Normalize(Vector3Subtract(sunPos, cam.position));

	Vector3 sunRDir = Vector3Normalize(Vector3CrossProduct(sunDir, Vector3{0.f, 1.f, 0.f}));
	Vector3 sunRiPos = Vector3Add(sunPos, Vector3Scale(sunRDir, sunSize));

	Vector2 sunScPos = GetWorldToScreenEx(sunPos, cam, wlength, wheight);
	Vector2 sunSidScPos = GetWorldToScreenEx(sunRiPos, cam, wlength, wheight);
	float sun_scsize = 3.5f * Vector2Distance(sunScPos, sunSidScPos);

	Vector3 planetRDir = Vector3Normalize(Vector3CrossProduct(Vector3Negate(cam.position), Vector3{0.f, 1.f, 0.f}));
	Vector3 planetRiPos = Vector3Scale(planetRDir, planetSize);
	Vector2 planetRiScPos = GetWorldToScreenEx(planetRiPos, cam, wlength, wheight);
	Vector2 planetScPos = GetWorldToScreenEx(Vector3Zero(), cam, wlength, wheight);
	float plan_scsize = 1.2f * Vector2Distance(planetScPos, planetRiScPos);

	float aspectRatio = wlength / (float)wheight;
	float fovx = (atanf(tanf(cam.fovy * 0.5) * aspectRatio)) * 1.15f; //stolen from internet

	float degRatio = acosf(Vector3DotProduct(lookDir, sunDir) / (Vector3Length(lookDir) * Vector3Length(sunDir)));
	float lookRatio = (1 - (degRatio / fovx));
	float brightness = (5 * lookRatio - (bright_offset*5.f)) * (-1 * lookRatio + (1-(bright_offset * 0.75f)));

	float difFromSunToPlanetSc = Vector2Distance(sunScPos, planetScPos);
	if (difFromSunToPlanetSc < (plan_scsize + sun_scsize)) {
		difFromSunToPlanetSc -= plan_scsize;
		difFromSunToPlanetSc /= sun_scsize;
		brightness *= max(difFromSunToPlanetSc, 0.f);
	}

	brightness = Clamp(brightness, 0.f, 1.f);

	if (brightness > 0.f) {
		Vector2 centerScreen = Vector2{wlength / 2.f, wheight / 2.f};
		Vector2 flareDir = Vector2Scale(Vector2Subtract(centerScreen, sunScPos), 2.f);
		//Vector2 flareNDir = Vector2Normalize(flareDir);
		float centerDistRatio = 1.1f * (sun_scsize / Vector2Distance(sunScPos, centerScreen));

		//Draw Astigmatism
		RenderFlare(sunScPos, sun_scsize, YELLOW, brightness);

		float ratios[4] = {0.33f, 0.66f, 0.75f, 0.95f};
		for (int i = 0; i < 4; i++) {
			float frac = ratios[i];
			Vector2 flareLocation = Vector2Add(sunScPos, Vector2Scale(flareDir, frac));
			float flareSize = Vector2Distance(flareLocation, centerScreen) * centerDistRatio;
			RenderFlare(flareLocation, flareSize, YELLOW, brightness);
		}
	}
}

int main(void) {
	InitWindow(wlength, wheight, "Lens Flare");

	Image flareImg = LoadImage("img/flare.png");
	ImageResize(&flareImg, (int)downsample, (int)downsample);
	flareTxr = LoadTextureFromImage(flareImg);
	UnloadImage(flareImg);

	Camera cam; 
	cam.fovy = 45.f;
	cam.projection = CAMERA_PERSPECTIVE;
	cam.up = Vector3{0.f, 1.f, 0.f};
	cam.target = Vector3Zero();
	cam.position = Vector3{70.f, 100.f, 0.f};

	float sunRad = 0.f;
	float sunRadius = 10.f;
	float sunSize = 15.f;

	SetTargetFPS(60);
	DisableCursor();
	while (!WindowShouldClose()) {
		UpdateCamera(&cam, CAMERA_FIRST_PERSON);

		//sunRad += GetFrameTime() * (PI / 3.f);
		Vector3 sunP = Vector3{40.f, -60.f, 0.f};

		BeginDrawing();
			ClearBackground(BLACK);
			BeginMode3D(cam);
				DrawSphere(sunP, sunSize, YELLOW);
				DrawSphere(Vector3Zero(), 15.f, RED);
				DrawGrid(20, 15.f);
			EndMode3D();
			LensFlareRender(cam, sunP, sunSize, 3);
			DrawFPS(10, 10);
		EndDrawing();
	}
}