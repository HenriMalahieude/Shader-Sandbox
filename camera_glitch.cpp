#include "raylib.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

const int wlength = 720;
const int wheight = wlength * 9 / 16;

int main() {
	InitWindow(wlength, wheight, "cam glitch");

	float factor = 0.01f;

	Image ded_pix = GenImageWhiteNoise(wlength, wheight, factor);
	ImageColorReplace(&ded_pix, BLACK, Color{0, 0, 0, 0});
	Texture2D dpTex = LoadTextureFromImage(ded_pix);

	while (!WindowShouldClose()) {
		int pressed = GetKeyPressed();

		if (pressed == KEY_Q || pressed == KEY_E) {
			if (pressed == KEY_Q) factor += 0.005f;
			if (pressed == KEY_E) factor -= 0.005f;
			factor = max(factor, 0.0f);

			cout << factor << endl;	

			UnloadImage(ded_pix);
			UnloadTexture(dpTex);
			ded_pix = GenImageWhiteNoise(wlength, wheight, factor); ImageColorReplace(&ded_pix, BLACK, Color{0, 0, 0, 0});
			dpTex = LoadTextureFromImage(ded_pix);
		}

		BeginDrawing();
			ClearBackground(WHITE);

			DrawTexture(dpTex, 0, 0, BLACK);

		EndDrawing();

	}

	return 0;
}