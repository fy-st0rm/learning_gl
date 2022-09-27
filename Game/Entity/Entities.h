#pragma once

#include "Entity.h"

#include <map>
#include <string>

struct Tile_IDs
{
	int tree = 37;
};


void Draw_Tree(const glm::vec2& pos, const glm::vec2& size, std::map<std::string, glm::vec4> tiles, const GL_Texture& map_texture);
