#include "raylib.h"
#include <iostream>
#include <vector>

typedef uint8_t u8;
typedef uint16_t u16;

typedef struct Mem_Block {
	Color color;

	u16 x_offset;
	u16 y_offset;

	u8 width;
	u8 height;

	bool selected;

	u8 value;
} Mem_Block;

std::vector<Mem_Block> memory;
std::vector<u16> cache(4, 0);
std::vector<Color> colors {
	ORANGE,
	BLUE,
	GREEN,
	YELLOW
};

void fill_memory(int cache_size, u8 x_offset, u8 y_offset, u8 width, u8 height) {
	for (u8 i = 0; i < 16; ++i) {
		Mem_Block block = Mem_Block {
			.x_offset = x_offset,
			.y_offset = y_offset + (i * height),

			.width = width,
			.height = height,

			.selected = false,

			.value = i,
		};
		
		memory.push_back(block);
	}
}

void try_select_memory(Vector2 mouse_pos) {
	for (int i = 0; i < memory.size(); ++i) {
		Mem_Block mem = memory[i];

		if (mouse_pos.x < mem.width && mouse_pos.y > mem.y_offset && mouse_pos.y <= mem.y_offset + mem.height) {
			Rectangle rec = Rectangle{(float)mem.x_offset, (float)mem.y_offset, (float)mem.width, (float)mem.height};			
				DrawRectangleLinesEx(rec, 5, RED);
				return;	
		}
	}
}

void handle_mouse_click() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mouse_pos = GetMousePosition();
		
		try_select_memory(mouse_pos);
		//try_select_cache(mouse_pos);
	}

}

void draw_memory() {
	int x_offset = 50;
	int y_offset = 100;

	DrawText("Memory", x_offset, (y_offset - 50) / 2, 50, RAYWHITE);

	int width = 200;
	int height = 50;

	int font_size = 30;
	int font_y_offset = (height - font_size) / 2;
	int font_x_offset = x_offset + (width - MeasureText("0x00", font_size)) / 2;

	for (int i = 0; i < memory.size(); ++i) {
		int y_offset_inc = y_offset + (i * 50);

		if (memory[i].value & 1) DrawRectangle(x_offset, y_offset_inc, width, height, LIGHTGRAY);
		else DrawRectangle(x_offset, y_offset_inc, width, height, GRAY);

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", memory[i].value);

		DrawText(hex_buffer, font_x_offset, y_offset_inc + font_y_offset, font_size, BLACK);
	}
}

void draw_cache() {
	int x_offset = 750;
	int y_offset = 100;

	DrawText("Cache", x_offset, (y_offset - 50) / 2, 50, RAYWHITE);

	int width = 200;
	int height = 50;

	int font_size = 30;
	int font_y_offset = (height - font_size) / 2;
	int font_x_offset = x_offset + (width - MeasureText("0x00", font_size)) / 2;

	for (int i = 0; i < cache.size(); ++i) {
		int y_offset_inc = y_offset + (i * 50);

		if (i & 1) DrawRectangle(x_offset, y_offset_inc, width, height, LIGHTGRAY);
		else DrawRectangle(x_offset, y_offset_inc, width, height, GRAY);

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", cache[i]);

		DrawText(hex_buffer, font_x_offset, y_offset_inc + font_y_offset, font_size, BLACK);
	}
}

int main() {
	u8 x_offset = 50;
	u8 y_offset = 100;

	u8 width = 200;
	u8 height = 50;

	fill_memory(4, x_offset, y_offset, width, height);

	SetTargetFPS(60);
	InitWindow(1000, 1000, "CPU Cache");

	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(Color{5, 5, 5, 255});
			
			draw_memory();
			draw_cache();
			handle_mouse_click();

			DrawFPS(0, 0);
		EndDrawing();

	}

	CloseWindow();

	return 1;

}