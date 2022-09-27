#pragma once

#include "Workspace.h"

#include "Globals.h"
#include "Dependency.h"

#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"

#include "Ship.h"

#include <math.h>


class WaterTest : public Workspace
{
private:
	Camera camera;
	Shader shader;

	float amplitude = 8.4f;
	float wave_no = 2.5f;
	float speed = 0.1f;
	float time = 0.0;;

	Ship ship;

	std::vector<glm::vec2> vertices;

public:
	WaterTest();
	~WaterTest();

	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	void draw_water();
	float get_y(float x);
}; 