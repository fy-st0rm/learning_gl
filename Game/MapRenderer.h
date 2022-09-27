#pragma once

#include "Globals.h"
#include "Dependency.h"
#include "Renderer.h"
#include "Entity.h"
#include "Camera.h"

#include "Entity/Entities.h"

#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <array>
#include <iostream>


class MapRenderer
{
private:
	
	// Some constant variables related to map thingy
	const static int map_width = 40;
	const static int map_height = 40;

	// Some texture an tile stuff
	GL_Texture map_texture;
	std::map<std::string, glm::vec4> tiles;

	// Some needed storage
	Camera* m_Camera;
	Entity* m_Ent;

	bool show_interaction_point = false;

public:
	std::vector<std::string> collidable_tiles_id = {"0", "37"};
	int tile_size[2] = { 16, 16 };

	// Actual map which stores the layes containing tiles id's
	std::vector<std::array<std::array<std::string, map_width>, map_height>> map;

	std::vector<glm::vec4> collidable_ent;

public:
	MapRenderer();
	~MapRenderer();

	void init(Camera* camera, Entity* ent);
	void clear_cache();
	void render(const glm::vec2& mouse_pos, int layer);
	void events();

private:
	void load_map(const std::string& map_path);
	void load_tiles();
};

