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

	DrawText("Memory", x_offset, (y_offset - 50) / 2, 50, BLACK);

	int width = 200;
	int height = 50;

	int font_size = 30;
	int font_y_offset = (height - font_size) / 2;
	int font_x_offset = x_offset + (width - MeasureText("0x00", font_size)) / 2;

	for (int i = 0; i < memory.size(); ++i) {
		int y_offset_inc = y_offset + (i * 50);

		if (memory[i] & 1) DrawRectangle(x_offset, y_offset_inc, width, height, LIGHTGRAY);
		else DrawRectangle(x_offset, y_offset_inc, width, height, GRAY);

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", memory[i]);

		DrawText(hex_buffer, font_x_offset, y_offset_inc + font_y_offset, font_size, BLACK);
	}
}

void draw_cache() {
	int x_offset = 750;
	int y_offset = 100;

	DrawText("Cache", x_offset, (y_offset - 50) / 2, 50, BLACK);

	int width = 200;
	int height = 50;

	int font_size = 30;
	int font_y_offset = (height - font_size) / 2;
	int font_x_offset = x_offset + (width - MeasureText("0x00", font_size)) / 2;

	for (int i = 0; i < cache.size(); ++i) {
		int y_offset_inc = y_offset + (i * 50);

		if (cache[i] & 1) DrawRectangle(x_offset, y_offset_inc, width, height, LIGHTGRAY);
		else DrawRectangle(x_offset, y_offset_inc, width, height, GRAY);

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", memory[i]);

		DrawText(hex_buffer, font_x_offset, y_offset_inc + font_y_offset, font_size, BLACK);
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
			ClearBackground(YELLOW);
			
			draw_memory();
			draw_cache();



			DrawFPS(0, 0);
			//DrawRectangle(225, 225, 50, 50, BLACK);
		EndDrawing();

	}

	CloseWindow();

	return 1;

}