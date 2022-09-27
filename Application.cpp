#include <iostream>

#include "Globals.h"
#include "Dependency.h"

#include "Workspace.h"
#include "Test/WaterTest.h"
#include "Game/Game.h"

#include "Input.h"
#include "Gui.h"


int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	// Some opengl stuff idk
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Hello World", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);									

	if (glewInit() != GLEW_OK)
		return -1;

	// Enabling some stuff
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	Renderer::init();
	Gui::Init(window);
	{
		// Scene Stuff
		Workspace* currentScene;
		Scenes* scene_manager = new Scenes(currentScene);
		currentScene = scene_manager;

		// Adding scenes
		scene_manager->AddScene<Game>("Game");
		scene_manager->AddScene<WaterTest>("Water Test");

		bool show_gui = true;

		double previous_tick = glfwGetTime();
		int fps = 0;

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			// Calculating FPS
			double current_tick = glfwGetTime();
			fps++;

			if (current_tick - previous_tick >= 1.0)
			{
				std::cout << "[FPS]: " << fps << std::endl;
				fps = 0;
				previous_tick = current_tick;
			}

			// Rendering scenes
			if (currentScene)
			{
				currentScene->OnEvent();
				currentScene->OnUpdate();

				if (show_gui)
				{
					Gui::BeginFrame();

					// Back functionality
					if (currentScene != scene_manager && ImGui::Button("<-"))
					{
						delete currentScene;
						currentScene = scene_manager;
					}

					currentScene->OnImGuiRender();
					
					Gui::EndFrame();
				}
			}


			// Key inputs
			// for opening manager
			if (input.key_pressed(GLFW_KEY_GRAVE_ACCENT))
			{
				if (show_gui)
					show_gui = false;
				else
					show_gui = true;
			}

			GLCall(glfwSwapBuffers(window));

			// Event callbacks
			glfwSetCursorPosCallback(window, cursor_position_callback);
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwPollEvents();
		}

		// Cleaning the scene stuff
		delete currentScene;
		if (currentScene != scene_manager)
			delete scene_manager;
	}

	Gui::ShutDown();
	Renderer::shutdown();

	glfwTerminate();
	return 0;
}
