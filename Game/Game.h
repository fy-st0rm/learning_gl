#pragma once

#include "Globals.h"
#include "Dependency.h"

#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "Input.h"
#include "Animator.h"

#include "Entity/Player.h"
#include "MapRenderer.h"

#include "Workspace.h"

#include <memory>
#include <array>

class Game : public Workspace
{
private:
	Camera camera;
	Shader shader;

	// Player
	Player player;
	
	// Map
	MapRenderer map_renderer;

	bool draw_hitbox = false;

public:
	Game();
	~Game();

	void OnEvent() override;
	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	void draw_colliders();

	glm::vec2 give_pointer();

};
