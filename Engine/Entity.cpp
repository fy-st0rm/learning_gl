#include "Entity.h"

Entity::Entity()
	:Position({0.0f, 0.0f}), Size({0.0f, 0.0f}), Offset({0.0f, 0.0f, 0.0f, 0.0f}), Texture({0, 0, 0})
{

}

Entity::~Entity()
{

}

void Entity::render()
{
	Renderer::draw_quad(Position, Size, Offset, Texture);
}

void Entity::static_render(const glm::vec4& color)
{
	Renderer::draw_quad(Position, Size, color);
}

void Entity::transform(const std::string& direction, float val)
{
	if (direction == "x")
		Position.x += val;
	else if (direction == "y")
		Position.y += val;
}

std::vector<glm::vec4> Entity::check_intersect(const glm::vec4& ent)
{
	// Drawing hitbox for debugging
	//Renderer::draw_quad({ ent.x, ent.y }, { ent.z, ent.w }, { 1.0f, 0.0f, 0.0f, 0.5f });
	
	std::vector<glm::vec4> hit_list;
	if (((Position.x <= ent.x) && (ent.x <= Position.x + Size.x)) || ((ent.x <= Position.x) && (Position.x <= ent.x + ent.z)))
		if (((Position.y <= ent.y) && (ent.y <= Position.y + Size.y)) || ((ent.y <= Position.y) && (Position.y <= ent.y + ent.w)))
			hit_list.push_back(ent);
	
	return hit_list;
}

void Entity::collide(const std::vector<glm::vec4>& collidable_ent)
{
	for (auto& i : collidable_ent)
	{
		auto hit_list = check_intersect(i);
		for (auto& rect : hit_list)
		{
			//Collision action here

			// Offsets of pixel in both x and y axis to cooperate x-axis and y-axis collider calculations (idk wat does this mean)
			int offsets[2] = { 5, 5 };

			// For left and right collision
			if (((Position.y <= rect.y) && (rect.y <= (Position.y + Size.y)-offsets[1])) || ((rect.y <= Position.y+offsets[1]) && (Position.y+offsets[1] <= rect.y + rect.w)))
			{
				// For left
				if ((Position.x <= rect.x) && (rect.x <= (Position.x + Size.x )))
					Position.x = rect.x - Size.x;

				// For right
				if ((rect.x <= Position.x) && (Position.x <= (rect.x + rect.z)))
					Position.x = rect.x + rect.z;

			}

			// For up and down collision
			if (((Position.x <= rect.x) && (rect.x <= (Position.x + Size.x)-offsets[0])) || ((rect.x <= Position.x+offsets[0]) && (Position.x+offsets[0] <= rect.x + rect.z)))
			{
				// For up
				if ((Position.y <= rect.y) && (rect.y <= (Position.y + Size.y)))
					Position.y = rect.y - Size.y;

				// For right
				if ((rect.y <= Position.y) && (Position.y <= (rect.y + rect.w)))
					Position.y = rect.y + rect.w;

			}
		}
	}
}
