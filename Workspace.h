#pragma once

#include <vector>
#include <functional>

#include "Dependency.h"


class Workspace
{
public:
	Workspace() {}
	virtual ~Workspace() {}

	virtual void OnEvent() {}
	virtual void OnUpdate() {}
	virtual void OnImGuiRender() {}
};


class Scenes : public Workspace
{
public:
	Scenes(Workspace*& currentScene);

	void OnImGuiRender() override;

	template <typename T>
	void AddScene(const std::string& name)
	{
		scenes.push_back(std::make_pair(name, []() { return new T(); }));
	}

private:
	Workspace*& m_CurrentScene;
	std::vector<std::pair<std::string, std::function<Workspace* ()>>> scenes;
};
