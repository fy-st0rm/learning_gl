#include "Entities.h"


void Draw_Tree(const glm::vec2& pos, const glm::vec2& size, std::map<std::string, glm::vec4> tiles, const GL_Texture& map_texture)
{
	Tile_IDs id;

	glm::vec2 position = pos;
	for (int i = 0; i < 2; i++)
	{
		Renderer::draw_quad(position, size, tiles[std::to_string(id.tree)], map_texture);

		position.y += size.y;
		id.tree--;
	}

	position.x += size.x;
	int x = 0;
	for (int i = 0; i < 12; i++)
	{
		Renderer::draw_quad(position, size, tiles[std::to_string(id.tree)], map_texture);
		x++;
		id.tree--;

		if (x < 3)
			position.x -= size.x;
		else
		{
			position.x = pos.x;
			position.x += size.x;
			position.y += size.y;
			x = 0;
		}
	}
}
