#include "raylib.h"
#include <iostream>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;

std::vector<u16> cache(4, 0);
std::vector<u8> memory(16, 0);

void draw_memory() {
	int x_offset = 50;
	int y_offset = 100;

	int width = 200;
	int height = 50;

	int font_y_offset = 5;
	//int font_x_offset = GET TEXT WIDTH FIRST
	int font_height = 40;

	for (int i = 0; i < memory.size(); ++i) {
		int y_offset_inc = y_offset + (i * 50);

		if (memory[i] & 1) DrawRectangle(x_offset, y_offset_inc, width, height, RED);
		else DrawRectangle(x_offset, y_offset_inc, width, height, GREEN);

		DrawText("0x0", x_offset, y_offset_inc + font_y_offset, font_height, BLACK);
	}	
}

void draw_cache() {
	int x_offset = 700;
	int y_offset = 100;

	int width = 200;
	int height = 50;

	int font_y_offset = 5;
	//int font_x_offset = GET TEXT WIDTH FIRST
	int font_height = 40;

	for (int i = 0; i < cache.size(); ++i) {
		int y_offset_inc = y_offset + (i * 50);

		if (cache[i] & 1) DrawRectangle(x_offset, y_offset_inc, width, height, RED);
		else DrawRectangle(x_offset, y_offset_inc, width, height, GREEN);

		DrawText("0x0", x_offset, y_offset_inc + font_y_offset, font_height, BLACK);
	}
}

int main() {

	for (int i = 0; i < cache.size(); ++i) {
		if (i & 1) cache[i] = 1;
	}

	for (int i = 0; i < memory.size(); ++i) {
		if (i & 1) memory[i] = 1;
	}

	SetTargetFPS(60);
	InitWindow(1000, 1000, "CPU Cache");

	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(RAYWHITE);
			
			draw_memory();
			draw_cache();



			DrawFPS(0, 0);
			//DrawRectangle(225, 225, 50, 50, BLACK);
		EndDrawing();

	}

	CloseWindow();

	return 1;

}