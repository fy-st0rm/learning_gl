#pragma once

#include "Dependency.h"


class Gui
{
public:
	static void Init(GLFWwindow* window);
	static void ShutDown();

	static void BeginFrame();
	static void EndFrame();
};
