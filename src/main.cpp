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
std::vector<Mem_Block> cache;
std::vector<Color> colors {
	ORANGE,
	BLUE,
	GREEN,
	MAGENTA
};

void fill_mem_array(int fill_size, u16 x_offset, u16 y_offset, u8 width, u8 height, std::vector<Mem_Block>& mem_array) {
	for (u8 i = 0; i < fill_size; ++i) {
		Mem_Block block = Mem_Block {
			.color = i & 1 ? RAYWHITE : OFFWHITE,

			.x_offset = x_offset,
			.y_offset = y_offset + (i * height),

			.width = width,
			.height = height,

			.selected = false,

			.value = 0,
		};

		mem_array.push_back(block);
	}
}

void try_select(Vector2 mouse_pos, int cache_size) {
	for (int i = 0; i < memory.size(); ++i) {
		Mem_Block& mem = memory[i];

		if (mouse_pos.x > mem.x_offset && mouse_pos.x < mem.x_offset + mem.width && mouse_pos.y > mem.y_offset && mouse_pos.y <= mem.y_offset + mem.height) {
			for (int j = 0; j < memory.size(); j++) {
				int cache_index = j % cache_size;

				Mem_Block& mem_block 	= memory[j];
				Mem_Block& cache_line	= cache[cache_index];

				if (cache_index == i % cache_size) {
					mem_block.selected 	= true;
					mem_block.color 	= colors[cache_index];
					mem_block.width 	= 225;

					cache_line.selected = true;
					cache_line.color 	= colors[cache_index];
				} else {
					mem_block.selected 	= false;
					mem_block.color 	= j & 1 ? RAYWHITE : OFFWHITE;
					mem_block.width 	= 200;

					cache_line.selected = false;
					cache_line.color 	= j & 1 ? RAYWHITE : OFFWHITE;
				}

			}
			
			return;
		}
	}

	//if nothing selected, reset all memory
	for (int i = 0; i < memory.size(); ++i) {
		Mem_Block& mem_block = memory[i];
		Mem_Block& cache_line = cache[i % cache_size];

		mem_block.color 	= i & 1 ? RAYWHITE : OFFWHITE;
		mem_block.selected 	= false;
		mem_block.width 	= 200;

		cache_line.color 	= i & 1 ? RAYWHITE : OFFWHITE;
		cache_line.selected = false;
	}
}

void draw_lines() {
	for (int i = 0; i < memory.size(); ++i) {
		Mem_Block mem = memory[i];
		
		if (mem.selected) {
			Vector2 start_pos 	= Vector2{(float)mem.x_offset + mem.width - 5, (float)mem.y_offset + (mem.height / 2)};
			Vector2 end_pos		= Vector2{(float)750, (float)125};

			DrawLineEx(start_pos, end_pos, 5, mem.color);
		}
	}
}

void handle_mouse_click() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mouse_pos = GetMousePosition();
		
		try_select(mouse_pos, 4);
		//try_select_cache(mouse_pos);
	}
}

//void draw_memory_name


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

		DrawRectangle(x_offset, y_offset_inc, cache[i].width, height, cache[i].color);

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", cache[i].value);

		DrawText(hex_buffer, font_x_offset, y_offset_inc + font_y_offset, font_size, BLACK);
	}
}

int main() {
	u16 memory_x_offset = 50;
	u16 cache_x_offset	= 750; 
	u16 y_offset 		= 100;

	u8 block_width 		= 200;
	u8 block_height 	= 50;

	fill_mem_array(16, memory_x_offset, y_offset, block_width, block_height, memory);
	fill_mem_array(4, cache_x_offset, y_offset, block_width, block_height, cache);

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