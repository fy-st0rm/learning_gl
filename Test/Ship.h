#pragma once

#include "Renderer.h"
#include "Globals.h"

class Ship
{
private:
	glm::vec2 position;
	glm::vec2 size;

	GL_Texture texture;

public:
	Ship();
	Ship(const glm::vec2& pos);
	~Ship();

	void set_pos(const glm::vec2& pos);
	void render();
	void update(const std::vector<glm::vec2>& vertices);
};