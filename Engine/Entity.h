#pragma once

#include "Dependency.h"
#include "Renderer.h"
#include "Globals.h"

#include <array>
#include <vector>

class Entity
{
public:
	glm::vec2 Position;
	glm::vec2 Size;
	glm::vec4 Offset;

	GL_Texture Texture;

public:
	Entity();
	~Entity();

	void render();
	void static_render(const glm::vec4& color);
	void transform(const std::string& direction, float val);
	
	// Collision system
	std::vector<glm::vec4> check_intersect(const glm::vec4& ent);
	void collide(const std::vector<glm::vec4>& collidable_ent);

};
