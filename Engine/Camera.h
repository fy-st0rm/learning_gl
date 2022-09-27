#pragma once

#include "Dependency.h"
#include "Input.h"
#include "Entity.h"

class Camera
{
private:
	glm::vec3 m_Position;
	glm::vec2 m_ScreenSize;

	bool zoom_in = false;
	bool zoom_out = false;

public:
	Camera();
	Camera(const glm::vec3& position, const glm::vec2& screenSize);
	~Camera();

	// Setters
	void setPos(const glm::vec3& position);
	void setSize(const glm::vec2& screenSize);

	// Getters
	inline glm::vec3 get_position() { return m_Position; }

	//Uitls function
	void follow(Entity* entity);
	void zoom();

	glm::mat4 getMVP();
};