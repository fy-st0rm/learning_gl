#include "Player.h"


Player::Player()
{
	Texture = Renderer::load_texture("Res/Textures/player.png");

	// Creating animation tree
	animation_tree["idle"]["left"]  = animator.create_database({ 6.0f, 0.0f, 16.0f, 16.0f }, 1, animation_speed);
	animation_tree["idle"]["right"] = animator.create_database({ 4.0f, 0.0f, 16.0f, 16.0f }, 1, animation_speed);
	animation_tree["idle"]["up"]    = animator.create_database({ 2.0f, 0.0f, 16.0f, 16.0f }, 1, animation_speed);
	animation_tree["idle"]["down"]  = animator.create_database({ 0.0f, 0.0f, 16.0f, 16.0f }, 1, animation_speed);
																								
	animation_tree["walk"]["left"]  = animator.create_database({ 6.0f, 0.0f, 16.0f, 16.0f }, 2, animation_speed);
	animation_tree["walk"]["right"] = animator.create_database({ 4.0f, 0.0f, 16.0f, 16.0f }, 2, animation_speed);
	animation_tree["walk"]["up"]    = animator.create_database({ 2.0f, 0.0f, 16.0f, 16.0f }, 2, animation_speed);
	animation_tree["walk"]["down"]  = animator.create_database({ 0.0f, 0.0f, 16.0f, 16.0f }, 2, animation_speed);
}

Player::~Player()
{
}

void Player::setRect(const glm::vec2& pos, const glm::vec2& size)
{
	Position = pos;
	Size = size;
}

void Player::update(Camera* camera, MapRenderer* map_renderer)
{
	events();
	interact(camera, map_renderer);

	transform("x", movementX);
	transform("y", movementY);

	// Giving the correct offset to draw
	std::vector<glm::vec4> database = animation_tree[animator.state][animator.side];
	Offset = database[animator.frame];

	animator.frame++;
	if (animator.frame >= database.size())
		animator.frame = 0;
}

void Player::events()
{
	if (input.key_hold(GLFW_KEY_W))
	{
		animator.change_state("walk");
		animator.change_side("up");

		movementX = 0;
		movementY = movement_speed;
	}
	else if (input.key_hold(GLFW_KEY_S))
	{
		animator.change_state("walk");
		animator.change_side("down");
		
		movementX = 0;
		movementY = -movement_speed;
	}
	else if(input.key_hold(GLFW_KEY_A))
	{
		animator.change_state("walk");
		animator.change_side("left");

		movementX = -movement_speed;
		movementY = 0;
	}
	else if (input.key_hold(GLFW_KEY_D))
	{
		animator.change_state("walk");
		animator.change_side("right");
		
		movementX = movement_speed;
		movementY = 0;
	}
	else
	{
		animator.change_state("idle");
		movementX = 0;
		movementY = 0;
	}

}

void Player::interact(Camera* camera, MapRenderer* map_renderer)
{
	collide(map_renderer->collidable_ent);
}
