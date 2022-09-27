#include "Ship.h"

Ship::Ship()
{
	texture = Renderer::load_texture("Res/Textures/ship.png");
}

Ship::Ship(const glm::vec2& pos)
	:position(pos)
{
	texture = Renderer::load_texture("Res/Textures/ship.png");
	size = { position.x + texture[1], position.y + texture[2] };
}

Ship::~Ship()
{

}

void Ship::set_pos(const glm::vec2& pos)
{
	position = pos;
	size = { position.x + texture[1], position.y + texture[2] };
}

void Ship::update(const std::vector<glm::vec2>& vertices)
{
	float px = position.x + 35;
	float py = position.y + 15;

	Renderer::draw_quad({ px, py }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });

	int i = 0;
	int next = 0;
	for (int current = 0; current < vertices.size(); current++)
	{
		next = current + 1;
		if (next >= vertices.size()) next = 0;

		glm::vec2 vc = vertices[current];
		glm::vec2 vn = vertices[next];

		if ((((vc.y >= py) && (vn.y < py)) || ((vc.y < py) && (vn.y >= py))) &&
			(px < (vn.x - vc.x) * (py - vc.y) / (vn.y - vc.y) + vc.x)) {
			std::cout << "Colliding " << i << std::endl;
			Renderer::draw_quad(vc, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
			Renderer::draw_quad(vn, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
			i++;
		}
	}
}

void Ship::render()
{
	Renderer::draw_polligon(position, { size.x, position.y }, size, { position.x, size.y }, texture[0]);
}