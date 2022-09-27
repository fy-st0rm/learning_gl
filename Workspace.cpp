#include "Workspace.h"


Scenes::Scenes(Workspace*& currentScene)
	:m_CurrentScene(currentScene)
{
}

void Scenes::OnImGuiRender()
{
	for (auto& scene : scenes)
	{
		if (ImGui::Button(scene.first.c_str()))
		{
			m_CurrentScene = scene.second();
		}
	}
}