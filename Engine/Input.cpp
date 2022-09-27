#include "Input.h"

std::map<int, int> Input::keys;
std::map<int, int> Input::buttons;
glm::vec2 Input::mouse_pos;


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Input::buttons[button] = action;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::keys[key] = action;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::mouse_pos.x = xpos;
	Input::mouse_pos.y = ypos;
}

bool Input::key_pressed(int key)
{
	if (Input::keys.find(key) != Input::keys.end())
	{
		if (Input::keys[key] == GLFW_PRESS)
		{
			Input::keys[key] = GLFW_RELEASE;
			return true;
		}
	}
	return false;
}

bool Input::key_hold(int key)
{
	if (Input::keys.find(key) != Input::keys.end())
	{
		if ((Input::keys[key] == GLFW_REPEAT) || (Input::keys[key] == GLFW_PRESS))
		{
			return true;
		}
	}
	return false;
}

bool Input::key_not_pressed(const std::vector<int>& keys)
{
	for (int i : keys)
	{
		if (Input::keys.find(i) != Input::keys.end())
		{
			if ((Input::keys[i] == GLFW_PRESS) || (Input::keys[i] == GLFW_REPEAT))
				return false;
		}
	}

	return true;
}

bool Input::mouse_button_pressed(int button)
{
	if (Input::buttons.find(button) != Input::buttons.end())
	{
		if (Input::buttons[button] == GLFW_PRESS)
		{
			Input::buttons[button] = GLFW_RELEASE;
			return true;
		}
	}
	return false;
}

