#include "MapRenderer.h"


MapRenderer::MapRenderer()
{
	map_texture = Renderer::load_texture("Res/Textures/spritesheet.png");

	load_map("Res/Textures/map5.txt");
	load_tiles();
}

MapRenderer::~MapRenderer()
{
}

void MapRenderer::init(Camera* camera, Entity* ent)
{
	m_Camera = camera;
	m_Ent = ent;
}

void MapRenderer::load_map(const std::string& map_path)
{
	std::fstream file(map_path);
	std::string line;

	int layer = -1;
	int y = map_height - 1;

	// Reading line by line of the map file
	while (std::getline(file, line))
	{
		if (line == "#newlayer")
		{
			// If its the new layer push the new map into the layers and reset the y offset
			std::array<std::array<std::string, map_width>, map_height> map_template;
			map.push_back(map_template);

			layer++;
			y = map_height - 1;
		}
		else
		{
			int x = 0;

			std::stringstream ch;
			for (auto& i : line)
			{
				if (i != ',')
				{
					ch << i;
				}
				else
				{
					map[layer][y][x] = ch.str(); 
					x++;
					ch.str(std::string());
					ch.clear();
				}
			}
			y--;
		}
	}
	file.close();
}

void MapRenderer::load_tiles()
{
	int index = 0;
	for (float y = (map_texture[2] / tile_size[1]) - 1; y >= 0; y--)
	{
		for (float x = 0.0f; x < (map_texture[1] / tile_size[0]); x++)
		{
			tiles[std::to_string(index)] = { x, y, tile_size[0], tile_size[1] };
			index++;
		}
	}
}

void MapRenderer::clear_cache()
{
	collidable_ent.clear();
}

void MapRenderer::render(const glm::vec2& mouse_pos, int layer)
{
	events();

	int offset[2] = { 9, 7 };
	int clamp_offset[2] = { 37, 39 };

	// The starting and ranging point of the for loops
	int x0 = int(m_Ent->Position.x / 32) - offset[0];
	int x1 = int(m_Ent->Position.x / 32) + offset[0];
	int y0 = int(m_Ent->Position.y / 32) - offset[1];
	int y1 = int(m_Ent->Position.y / 32) + offset[1];

	// Clamping numbers to clamp the outside tiles
	int clampX0 = int(m_Ent->Position.x - ZOOM_WIDTH / 2) - clamp_offset[0];
	int clampX1 = int(m_Ent->Position.x + ZOOM_WIDTH / 2) + clamp_offset[0];
	int clampY0 = int(m_Ent->Position.y - ZOOM_HEIGHT / 2) - clamp_offset[1];
	int clampY1 = int(m_Ent->Position.y + ZOOM_HEIGHT / 2) + clamp_offset[1];

	//std::cout << x0 << " " << x1 << " | " << y0 << " " << y1 << std::endl;
	//std::cout << ent->Position.x << " " << ent->Position.y << " | " << camera->get_position().x << " " << camera->get_position().y << std::endl;

	for (int y = y0; y < y1; y++)
	{
		for (int x = x0; x < x1; x++)
		{
			if (((0 <= y) && (y < map_height)) && ((0 <= x) && (x < map_width)))
			{
				std::string tile = map[layer][y][x];
				glm::vec2 pos = { x * 32, y * 32 };
				glm::vec2 size = { 32, 32 };

				// Render the sprites which r on the screen
				if (((clampX0 <= pos.x) && (pos.x < clampX1)) || ((pos.x <= clampX0) && (clampX0 < pos.x + size.x)))
				{
					if (((clampY0 <= pos.y) && (pos.y < clampY1)) || ((pos.y <= clampY0) && (clampY0 < pos.y + size.y)))
					{
						// Rendering here:
						if ((tile != "18") && (tile != "-1"))
						{
							if (show_interaction_point)
								if ((mouse_pos.x == x) && (mouse_pos.y == y))
									Renderer::BeginTint({ 0.0f, 1.0f, 0.0f, 0.5f });
							
							Renderer::draw_quad(pos, size, tiles[tile], map_texture);
							Renderer::EndTint();
						}

						for (auto& i : collidable_tiles_id)
						{
							if (tile == i)
							{
								collidable_ent.push_back({ pos, size });
							}
						}
					}
				}
			}
		}
	}

}

void MapRenderer::events()
{
	if (input.mouse_button_pressed(GLFW_MOUSE_BUTTON_2))
	{
		if (show_interaction_point)
			show_interaction_point = false;
		else
			show_interaction_point = true;
	}
}
