#include "Camera.h"

Camera::Camera()
	:m_Position({0.0f, 0.0f, 0.0f})
{
}

Camera::Camera(const glm::vec3& position, const glm::vec2& screenSize)
	:m_Position(position), m_ScreenSize(screenSize)
{
}

Camera::~Camera()
{
}

void Camera::setPos(const glm::vec3& position)
{
	m_Position = position;
}

void Camera::setSize(const glm::vec2& screenSize)
{
	m_ScreenSize = screenSize;
}

void Camera::follow(Entity* entity)
{
//	m_Position = { entity->Position, 0.0f };
//	m_Position.x -= (m_ScreenSize.x / 2) - (entity->Size.x / 2);
//	m_Position.y -= (m_ScreenSize.y / 2) - (entity->Size.y / 2);

	m_Position.x += (entity->Position.x - m_Position.x - ZOOM_WIDTH / 2) / 10;
	m_Position.y += (entity->Position.y - m_Position.y - ZOOM_HEIGHT / 2) / 10;
}

void Camera::zoom()
{
	if (input.key_hold(GLFW_KEY_Q))
		zoom_in = true;
	else
		zoom_in = false;
	
	if (input.key_hold(GLFW_KEY_E))
		zoom_out = true;
	else
		zoom_out = false;

	if (zoom_in)
	{
		m_ScreenSize.x -= 1;
		m_ScreenSize.y -= 1;
	}
	else if (zoom_out)
	{
		m_ScreenSize.x += 1;
		m_ScreenSize.y += 1;
	}

	std::cout << m_ScreenSize.x << " " << m_ScreenSize.y << std::endl;
}

glm::mat4 Camera::getMVP()
{
	//std::cout << m_Position.y << std::endl;

	glm::mat4 proj = glm::ortho(0.0f, (float)m_ScreenSize.x, 0.0f, (float)m_ScreenSize.y, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position.x * -1, m_Position.y * -1, 0.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	glm::mat4 mvp = proj * view * model;
	return mvp;
}

