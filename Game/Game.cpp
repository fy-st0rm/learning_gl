#include "Game.h"


Game::Game()
{
	camera.setPos({ 0.0f, 0.0f, 0.0f });
	camera.setSize({ ZOOM_WIDTH, ZOOM_HEIGHT });

	shader.create("Res/Shader/vertex.vs", "Res/Shader/fragment.fs");
	shader.Bind();

	int samplers[MaxTextureSlot];

	for(int i = 0; i < MaxTextureSlot; i++)
	{
		samplers[i] = i;
	}
	shader.SetUniform1iv("u_Textures", MaxTextureSlot, samplers);

	// Player setup
	player.setRect({ 160.0f, 160.0f }, { 32.0f, 32.0f });

	map_renderer.init(&camera, &player);
}

Game::~Game()
{
}

void Game::draw_colliders()
{
	if (draw_hitbox)
	{
		for (auto& i : map_renderer.collidable_ent)
		{
			Renderer::draw_quad({ i.x, i.y }, { i.z, i.w }, { 1.0f, 0.0f, 0.0f, 0.5f });
		}
	}
}

glm::vec2 Game::give_pointer()
{
	float ratio_x = (WIN_WIDTH / ZOOM_WIDTH);
	float ratio_y = (WIN_HEIGHT / ZOOM_HEIGHT);
	glm::vec2 new_pos = { input.mouse_pos.x / ratio_x, ZOOM_HEIGHT - input.mouse_pos.y / ratio_y };

	auto cam = camera.get_position();
	glm::vec2 pos = { int(((new_pos.x) + cam.x) / 32), int(((new_pos.y) + cam.y) / 32) };

	return pos;
}

void Game::OnEvent()
{
}

void Game::OnUpdate()
{
	Renderer::Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

	// Camera actions
	shader.SetUniformMat4f("u_MVP", camera.getMVP());
	camera.follow(&player);

	// Rendering stuff
	Renderer::ResetStats();
	Renderer::BeginBatch();
	
	map_renderer.clear_cache();

	map_renderer.render(give_pointer(), 0);
	player.render();
	map_renderer.render(give_pointer(), 1);

	player.update(&camera, &map_renderer);

	draw_colliders();

	Renderer::EndBatch();
	Renderer::Flush();
}

void Game::OnImGuiRender()
{
	// Renderer stats
	auto stats = Renderer::GetStats();
	std::string draw_call = "Draw Calls: " + std::to_string(stats.draw_count);
	std::string quad_count = "Quads: " + std::to_string(stats.quad_count);

	ImGui::Text(draw_call.c_str());
	ImGui::Text(quad_count.c_str());

	if (ImGui::Button("HitBox"))
	{
		if (draw_hitbox)
			draw_hitbox = false;
		else
			draw_hitbox = true;
	}
}