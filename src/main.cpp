#include "raylib.h"
#include <iostream>
#include <vector>

Color orange	= Color{239, 156, 102, 255};
Color yellow	= Color{252, 220, 148, 255};
Color green 	= Color{200, 207, 160, 255};
Color blue 		= Color{120, 171, 168, 255};

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

	u8 tag;
} Mem_Block;

Color OFFWHITE = Color{220, 220, 220, 255};

u8 instruction_pointer = 0;

std::vector<Mem_Block> ram;
std::vector<Mem_Block> cache;
std::vector<Color> colors {
	orange,
	yellow,
	green,
	blue
};

void fill_mem_array(int fill_size, u16 x_offset, u16 y_offset, u8 width, u8 height, std::vector<Mem_Block>& mem_array) {
	for (int i = 0; i < fill_size; ++i) {
		Mem_Block block = Mem_Block {
			.color = i & 1 ? RAYWHITE : OFFWHITE,

			.x_offset = x_offset,
			.y_offset = y_offset + (i * height),

			.width = width,
			.height = height,

			.selected = false,

			.value = 0,

			.tag = (0b00001100 & i) >> 2
		};

		mem_array.push_back(block);
	}
}

void try_select(Vector2 mouse_pos, int cache_size) {
	Mem_Block mem = ram[0];

	//check RAM
	if (mouse_pos.x > mem.x_offset && 
		mouse_pos.x < mem.x_offset + mem.width && 
		mouse_pos.y > mem.y_offset && 
		mouse_pos.y <= mem.y_offset + (mem.height * ram.size())
	) {
		int selected_index = (mouse_pos.y - mem.y_offset) / mem.height;

		cache[selected_index % cache_size].tag = ram[selected_index].tag;

		for (int j = 0; j < ram.size(); j++) {
			int cache_index = j % cache_size;

			Mem_Block& mem_block 	= ram[j];
			Mem_Block& cache_line	= cache[cache_index];
			
			if (cache_index == selected_index % cache_size) {
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

	//if nothing selected, reset all memory
	for (int i = 0; i < ram.size(); ++i) {
		Mem_Block& mem_block = ram[i];
		Mem_Block& cache_line = cache[i % cache_size];

		mem_block.color 	= i & 1 ? RAYWHITE : OFFWHITE;
		mem_block.selected 	= false;
		mem_block.width 	= 200;

		cache_line.color 	= i & 1 ? RAYWHITE : OFFWHITE;
		cache_line.selected = false;
	}
}

//needs improvement
void draw_lines(u16 cache_x_offset) {
	for (int i = 0; i < ram.size(); ++i) {
		Mem_Block mem = ram[i];
		
		if (mem.selected) {
			int cache_index = i % 4;
			int y_end = 100 + (cache_index * 50) + 25;

			Vector2 start_pos 	= Vector2{(float)mem.x_offset + mem.width - 5, (float)mem.y_offset + (mem.height / 2)};
			Vector2 end_pos		= Vector2{(float)cache_x_offset, (float)y_end};

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

void draw_ram(u8 block_width) {
	for (int i = 0; i < ram.size(); ++i) {
		Mem_Block mem = ram[i];

		//Draw Index
		Color color = colors[i % 4];

		DrawRectangle(mem.x_offset - 70, mem.y_offset, 70, mem.height, color);
		
		int font_size = 20;
		int font_y_offset = (mem.height - font_size) / 2;

		u8 value = (u8)i;

		char index_buffer[5];
		for (int i = 3; i >= 0; --i) {
			index_buffer[i] = (value & 1) ? '1' : '0';
			value >>= 1;
		}

		//center index in rect
		int font_x_offset = (mem.x_offset - 70) + (70 - MeasureText(index_buffer, font_size)) / 2;
		DrawText(index_buffer, font_x_offset, mem.y_offset + font_y_offset, font_size, BLACK);

		//Draw Memory Block
		DrawRectangle(mem.x_offset, mem.y_offset, mem.width, mem.height, mem.color);

		font_size = 30;
		font_y_offset = (mem.height - font_size) / 2;
		font_x_offset = mem.x_offset + (block_width - MeasureText("0x00", font_size)) / 2;

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", mem.value);

		DrawText(hex_buffer, font_x_offset, mem.y_offset + font_y_offset, font_size, BLACK);
	}
}


void draw_cache(u8 block_width) {
	for (int i = 0; i < cache.size(); ++i) {
		Mem_Block mem = cache[i];

		DrawRectangle(mem.x_offset, mem.y_offset, mem.width, mem.height, mem.color);

		int font_size = 30;
		int font_y_offset = (mem.height - font_size) / 2;
		int font_x_offset = mem.x_offset + (block_width - MeasureText("0x00", font_size)) / 2;

		char hex_buffer[9];
		auto hex_code = sprintf(hex_buffer, "0x%02X", mem.value);

		DrawText(hex_buffer, font_x_offset, mem.y_offset + font_y_offset, font_size, BLACK);

		if (mem.selected) {
			int box_width = 98;
			int label_y_offset = mem.y_offset + (mem.height * (cache.size() - i)) + 5;

			int value_x_offset_left 	= mem.x_offset;
			int value_x_offset_right 	= mem.x_offset + (mem.width - box_width);
			int value_y_offset 			= label_y_offset + (mem.height / 1.5) + 5;
			
			//Draw Label Boxes
			DrawRectangle(mem.x_offset, label_y_offset, box_width, mem.height / 1.5, mem.color);
			DrawRectangle(mem.x_offset + (mem.width - box_width), label_y_offset, box_width, mem.height / 1.5, mem.color);

			//Draw Value boxes
			DrawRectangle(value_x_offset_left, value_y_offset, box_width, mem.height, mem.color);
			DrawRectangle(value_x_offset_right, value_y_offset, box_width, mem.height, mem.color);

			//Draw Cache Line Index
			u8 index = (u8)i;
			char buffer[3];
			for (int j = 1; j >= 0; --j) {
				buffer[j] = index & 1 ? '1' : '0';
				index >>= 1;
			}
			buffer[2] = '\0';
			
			font_size = 20;
			font_y_offset = (mem.height - font_size) / 2;
			font_x_offset = mem.x_offset + (box_width - MeasureText(buffer, font_size)) / 2;

			DrawText(buffer, font_x_offset, value_y_offset + font_y_offset, font_size, BLACK);

			//Draw Index Label
			int index_font_x = mem.x_offset + (box_width - MeasureText("Index", font_size)) / 2;
			int index_font_y = label_y_offset + (mem.height / 1.5 - font_size) / 2;

			DrawText("Index", index_font_x, index_font_y, font_size, BLACK);

			//Draw Tag Label
			int tag_font_x = mem.x_offset + 102 + (box_width - MeasureText("Tag", font_size)) / 2;
			int tag_font_y = label_y_offset + (mem.height / 1.5 - font_size) / 2;

			DrawText("Tag", tag_font_x, tag_font_y, font_size, BLACK);

			//Draw Cache Value Tag
			if (mem.value > 0) {
				u8 tag = mem.tag;

				for (int j = 1; j >= 0; --j) {
					buffer[j] = tag & 1 ? '1' : '0';
					tag >>= 1;
				}
				buffer[2] = '\0';
			
				font_size = 20;
				font_y_offset = (mem.height - font_size) / 2;
				font_x_offset = mem.x_offset + 102 + (box_width - MeasureText(buffer, font_size)) / 2;

				DrawText(buffer, font_x_offset, value_y_offset + font_y_offset, font_size, BLACK);
			}
		}
	}
}

void draw_instruction_text(const char* text, int x_offset, int y_offset, int inst_box_width, int inst_box_height, int font_size, Color color) {
	int text_width 	= MeasureText(text, font_size);
	int adjusted_x 	= x_offset + ((inst_box_width - text_width) / 2);
	int adjusted_y	= y_offset + ((inst_box_height - font_size) / 2);

	DrawText(text, adjusted_x, adjusted_y, font_size, color);
}

void draw_instructions(u16 x_offset, u16 y_offset, u16 inst_box_width, u16 inst_box_height, u16 inst_font_size) {
	int ip 		= instruction_pointer;
	Color color = BLACK;

	for (int i = 0; i < 20; ++i) {
		if (ip == i) color = RED;

		int adjusted_y = y_offset + (i * inst_box_height);

		if (i & 1) DrawRectangle(x_offset, adjusted_y, inst_box_width, inst_box_height, WHITE);
		else DrawRectangle(x_offset, adjusted_y, inst_box_width, inst_box_height, LIGHTGRAY);

		if (i == 0) {
			draw_instruction_text("int a = 5", x_offset, adjusted_y, inst_box_width, inst_box_height, inst_font_size, color);
		}

		if (i == 1) {
			draw_instruction_text("int b = 3", x_offset, adjusted_y, inst_box_width, inst_box_height, inst_font_size, color);
		}

		if (i == 2) {
			draw_instruction_text("int c = a + b", x_offset, adjusted_y, inst_box_width, inst_box_height, inst_font_size, color);
		}
	}
}

void draw_instruction_pointer(float x_offset, float y_offset, float inst_box_width, float inst_box_height) {
	DrawRectangleLinesEx(Rectangle{x_offset, y_offset + (instruction_pointer * inst_box_height), inst_box_width, inst_box_height}, 3, RED);
}

void process_instruction_and_inc_instruction_pointer() {
	u8& i = instruction_pointer;

	if (i == 0) {
		ram[0].value = 5;
	}

	if (i == 1) {
		ram[1].value = 3;
	}

	if (i == 2) {
		cache[0].value = 5;
		cache[1].value = 3;
		ram[2].value = 8;
	}

	instruction_pointer++;
}

void process_instruction() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		process_instruction_and_inc_instruction_pointer();
	}	
}

void draw_name(const char* name, u16 x_offset, u16 y_offset, int font_size) {
	DrawText(name, x_offset, (y_offset - font_size) / 2, font_size, RAYWHITE);
}

int main() {
	u16 inst_x_offset	= 700;
	u16 ram_x_offset 	= 100;
	u16 cache_x_offset	= 400; 
	u16 y_offset 		= 100;

	u8 block_width 		= 200;
	u8 block_height 	= 50;

	u16 inst_box_width	= 300;
	u16 inst_box_height	= 40;
	u16 inst_font_size	= 30;

	fill_mem_array(16, ram_x_offset, y_offset, block_width, block_height, ram);
	fill_mem_array(4, cache_x_offset, y_offset, block_width, block_height, cache);

	SetTargetFPS(60);
	InitWindow(1050, 1000, "CPU Cache");

	while (!WindowShouldClose()) {

		BeginDrawing();
			ClearBackground(Color{5, 5, 5, 255});
			
			draw_name("RAM", ram_x_offset, y_offset, 50);
			draw_name("Cache", cache_x_offset, y_offset, 50);
			draw_name("Instructions", inst_x_offset, y_offset, 50);

			process_instruction();

			draw_ram(block_width);
			draw_cache(block_width);
			draw_instructions(inst_x_offset, y_offset, inst_box_width, inst_box_height, inst_font_size);
			draw_instruction_pointer(inst_x_offset, y_offset, inst_box_width, inst_box_height);
			handle_mouse_click();

			DrawFPS(0, 0);

			draw_lines(cache_x_offset);
		EndDrawing();

	}

	CloseWindow();

	return 1;

}