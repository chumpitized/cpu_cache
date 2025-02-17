#include "raylib.h"
#include <iostream>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;

std::vector<u16> cache(4);
std::vector<u8> memory(16);

int main() {

	SetTargetFPS(60);
	InitWindow(500, 500, "CPU Cache");

	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawFPS(0, 0);
			DrawRectangle(225, 225, 50, 50, BLACK);
		EndDrawing();

	}

	CloseWindow();

	return 1;

}