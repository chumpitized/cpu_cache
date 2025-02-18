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

Color OFFWHITE = Color{220, 220, 220, 255};

std::vector<Mem_Block> memory;
std::vector<u16> cache(4, 0);
std::vector<Color> colors {
	ORANGE,
	BLUE,
	GREEN,
	MAGENTA
};

void fill_memory(int cache_size, u8 x_offset, u8 y_offset, u8 width, u8 height) {
	for (u8 i = 0; i < 16; ++i) {
		Mem_Block block = Mem_Block {
			.color = i & 1 ? RAYWHITE : OFFWHITE,

			.x_offset = x_offset,
			.y_offset = y_offset + (i * height),

			.width = width,
			.height = height,

			.selected = false,

			.value = 0,
		};

		memory.push_back(block);
	}
}



void try_select_memory(Vector2 mouse_pos, int cache_size) {
	for (int i = 0; i < memory.size(); ++i) {
		Mem_Block& mem = memory[i];

		if (mouse_pos.x > mem.x_offset && mouse_pos.x < mem.x_offset + mem.width && mouse_pos.y > mem.y_offset && mouse_pos.y <= mem.y_offset + mem.height) {
			for (int j = 0; j < memory.size(); j++) {
				Mem_Block& mem = memory[j];

				if (j % cache_size == i % cache_size) {
					mem.selected = true;
					mem.color = colors[(j % cache_size)];
					mem.width = 225;
				} else {
					mem.selected = false;
					mem.color = j & 1 ? RAYWHITE : OFFWHITE;
					mem.width = 200;
				}

			}
			
			return;
		}
	}


	//if nothing selected, reset all memory
	for (int i = 0; i < memory.size(); ++i) {
		memory[i].color = i & 1 ? RAYWHITE : OFFWHITE;
		memory[i].selected = false;
		memory[i].width = 200;
	}
}

void draw_lines() {
	for (int i = 0; i < memory.size(); ++i) {
		Mem_Block mem = memory[i];
		
		if (mem.selected) {
			Vector2 start_pos 	= Vector2{mem.x_offset + mem.width - 5, mem.y_offset + (mem.height / 2)};
			Vector2 end_pos		= Vector2{750, 125};

			DrawLineEx(start_pos, end_pos, 5, mem.color);
		}
	}
}

void handle_mouse_click() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mouse_pos = GetMousePosition();
		
		try_select_memory(mouse_pos, 4);
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

		DrawRectangle(x_offset, y_offset_inc, memory[i].width, height, memory[i].color);

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

			draw_lines();
		EndDrawing();

	}

	CloseWindow();

	return 1;

}