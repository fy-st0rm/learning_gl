#pragma once

#include "Dependency.h"
#include "Entity.h"
#include "Animator.h"
#include "Input.h"
#include "Camera.h"
#include "../MapRenderer.h"

#include <vector>
#include <map>
#include <string>

class Player : public Entity
{
private:
	// Animation stuff
	Animator animator;
	std::map<std::string, std::map<std::string, std::vector<glm::vec4>>> animation_tree;
	
	int animation_speed = 1;
	
	float movement_speed = 4.0f;
	int movementX = 0;
	int movementY = 0;

public:
	Player();
	~Player();

	// Setters
	void setRect(const glm::vec2& pos, const glm::vec2& size);

	void update(Camera* camera, MapRenderer* map_renderer);

private:
	void events();
	void interact(Camera* camera, MapRenderer* map_renderer);
};