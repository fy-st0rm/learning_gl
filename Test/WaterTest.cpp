#include "WaterTest.h"

WaterTest::WaterTest()
{
	camera.setPos({ 0.0f, 0.0f, 0.0f });
	camera.setSize({ ZOOM_WIDTH, ZOOM_HEIGHT });

	shader.create("Res/Shader/vertex.vs", "Res/shader/fragment.fs");
	shader.Bind();

	int samplers[MaxTextureSlot];
	for (int i = 0; i < MaxTextureSlot; i++)
	{
		samplers[i] = i;
	}
	shader.SetUniform1iv("u_Textures", MaxTextureSlot, samplers);


	ship.set_pos({ 100.0f, 90.0f });
}

WaterTest::~WaterTest()
{

}

float WaterTest::get_y(float x)
{
	float y = amplitude * std::sin(wave_no * x - speed * time + 10.0f);
	time += 0.05f;
	return y;
}

void WaterTest::draw_water()
{
	vertices.clear();
	float  x = 0;
	for (int i = 0; i < 8; i++)
	{
		std::vector<glm::vec2> water_pos;
		
		for (int j = 0; j < 2; j++)
		{
			float y = get_y(x) + 100.0f;
			water_pos.push_back({ x, y });
			x += 50.0f; 
		}

		x -= 50.0f;


		Renderer::draw_polligon({ water_pos[0].x, 0.0f }, { water_pos[1].x, 0.0f }, { water_pos[1].x, water_pos[1].y }, { water_pos[0].x, water_pos[0].y }, { 0.5f, 0.8f, 1, 0.5f });
		
		vertices.push_back({ water_pos[0].x, 0.0f });
		vertices.push_back({ water_pos[1].x, 0.0f });
		vertices.push_back({ water_pos[1].x, water_pos[1].y });
		vertices.push_back({ water_pos[0].x, water_pos[0].y });
	}
}

void WaterTest::OnUpdate()
{
	Renderer::Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

	// MVP
	shader.SetUniformMat4f("u_MVP", camera.getMVP());

	Renderer::ResetStats();
	Renderer::BeginBatch();

	ship.render();
	draw_water();
	ship.update(vertices);

	Renderer::EndBatch();
	Renderer::Flush();
}

void WaterTest::OnImGuiRender()
{
	// Renderer stats
	auto stats = Renderer::GetStats();
	std::string draw_call = "Draw Calls: " + std::to_string(stats.draw_count);
	std::string quad_count = "Quads: " + std::to_string(stats.quad_count);

	ImGui::Text(draw_call.c_str());
	ImGui::Text(quad_count.c_str());
}