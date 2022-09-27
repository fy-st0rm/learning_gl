#pragma once

#include "Dependency.h"

#include <map>
#include <vector>

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

struct Input
{
	static std::map<int, int> keys;
	static std::map<int, int> buttons;
	static glm::vec2 mouse_pos;

	// Keyboard functions
	bool key_pressed(int key);
	bool key_hold(int key);
	bool key_not_pressed(const std::vector<int>& keys);

	// Mouse functions
	bool mouse_button_pressed(int button);
};

static Input input;
