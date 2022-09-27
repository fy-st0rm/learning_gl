#pragma once

#include "Dependency.h"

#include "Shader.h"

#include <array>
#include <vector>
#include <memory>
#include <algorithm>


struct Vertex
{
	glm::vec2 position;
	glm::vec2 texCoords;
	glm::vec4 color;
	float texID;
};

// Some constants references
const int MaxQuadCount = 1000;
const int MaxIndexCount = MaxQuadCount * 6;
const int MaxVertexBufferSize = MaxQuadCount * sizeof(Vertex) * 4;
const int MaxTextureSlot = 32;

// Rendering data storage
struct RendererData
{
	// Buffer objects IDs
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	int index_count = 0;

	// Quad storage
	std::vector<std::array<Vertex, 4>> TemBuffer;
	std::vector<float> QuadBuffer;

	unsigned int white_texture = 0;
	unsigned int texture_slot_index = 1;
	std::array<unsigned int, MaxTextureSlot> texture_slots;

	// Color
	glm::vec4 def_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};
static RendererData r_Data;


class Renderer
{
public:
	static void init();
	static void shutdown();

	static void BeginBatch();
	static void EndBatch();
	static void Flush();

	static void Clear(const glm::vec4& color);

	// Tint functions
	static void BeginTint(const glm::vec4& color);
	static void EndTint();

	// Quads
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, unsigned int texture);
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& offset, const std::array<unsigned int, 3>& texture);
	
	static void draw_polligon(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec4& color);
	static void draw_polligon(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, unsigned int texture);

	// Texture
	static std::array<unsigned int, 3> load_texture(const std::string& path, int vertical_flip=1);
	static void bind_texture(unsigned int id);

	// Statistics
	struct Stat
	{
		int quad_count = 0;
		int draw_count = 0;
	};

	static void ResetStats();
	static Stat GetStats();
};
