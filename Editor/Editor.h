#pragma once

#include "Dependency.h"
#include "Workspace.h"
#include "Renderer.h"
#include "Shader.h"
#include "SpriteSheet.h"
#include "Input.h"
#include "Camera.h"


class Editor : public Workspace
{
private:
	Renderer renderer;
	SpriteSheet spritesheet;
	Camera camera;

	const int m_Width = 20;
	const int m_Height = 20;

	int** map;

public:
	Editor();
	~Editor();

	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	void create_map();
	void print_map();
	void add_tiles();
	void delete_tiles();

	void draw_tiles();

};