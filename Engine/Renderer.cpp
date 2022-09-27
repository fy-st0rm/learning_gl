#include "Renderer.h"

#include "stb_image/stb_image.h"
#include <fstream>


static Renderer::Stat renderer_stat;

/////////////////////Renderer initialization and main mechanism/////////////////////

void Renderer::init()
{
	// Creating vertex array
	GLCall(glGenVertexArrays(1, &r_Data.VAO));
	GLCall(glBindVertexArray(r_Data.VAO));

	// Creating vertex buffer
	GLCall(glGenBuffers(1, &r_Data.VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, r_Data.VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexBufferSize, nullptr, GL_DYNAMIC_DRAW));

	// Creating layout for the buffer
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords)));

	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color)));

	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID)));

	// Generating indices
	unsigned int indices[MaxIndexCount];
	for (int offset = 0, i = 0; i < MaxIndexCount; i += 6)
	{
		indices[0 + i] = 0 + offset;
		indices[1 + i] = 1 + offset;
		indices[2 + i] = 2 + offset;

		indices[3 + i] = 2 + offset;
		indices[4 + i] = 3 + offset;
		indices[5 + i] = 0 + offset;
		offset += 4;
	}

	GLCall(glGenBuffers(1, &r_Data.IBO));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r_Data.IBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// Generating a 1x1 white texture
	GLCall(glCreateTextures(GL_TEXTURE_2D, 1, &r_Data.white_texture));
	GLCall(glBindTexture(GL_TEXTURE_2D, r_Data.white_texture));

	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	unsigned int white = 0xffffffff;
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white));

	r_Data.texture_slots[0] = r_Data.white_texture;
	for (int i = 1; i < MaxTextureSlot; i++)
	{
		r_Data.texture_slots[i] = 0;
	}
}

void Renderer::shutdown()
{
	GLCall(glDeleteVertexArrays(1, &r_Data.VAO));
	GLCall(glDeleteBuffers(1, &r_Data.VBO));
	GLCall(glDeleteBuffers(1, &r_Data.IBO));

	GLCall(glDeleteTextures(1, &r_Data.white_texture));

	for (auto& i : r_Data.texture_slots)
	{
		GLCall(glDeleteTextures(1, &i));
	}
}

void Renderer::BeginBatch()
{
	r_Data.QuadBuffer.clear();
}

void Renderer::EndBatch()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, r_Data.VBO));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, r_Data.QuadBuffer.size() * sizeof(float), r_Data.QuadBuffer.data()));
}

void Renderer::Flush()
{
	for (unsigned int i = 0; i < r_Data.texture_slot_index; i++)
	{ 
		GLCall(glBindTextureUnit(i, r_Data.texture_slots[i]));
	}

	GLCall(glBindVertexArray(r_Data.VAO));
	GLCall(glDrawElements(GL_TRIANGLES, r_Data.index_count, GL_UNSIGNED_INT, nullptr));
	
	renderer_stat.draw_count++;

	r_Data.index_count = 0;
}

void Renderer::Clear(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);

}

/////////////////////Tinting/////////////////////
void Renderer::BeginTint(const glm::vec4& color)
{
	r_Data.def_color = color;
}

void Renderer::EndTint()
{
	r_Data.def_color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

/////////////////////Texture Stuff/////////////////////

std::array<unsigned int, 3> Renderer::load_texture(const std::string& path, int vertical_flip)
{
	if (vertical_flip)
		stbi_set_flip_vertically_on_load(vertical_flip);

	// Temp vars
	unsigned int ID;
	unsigned char* m_LocalBuffer = nullptr;
	int m_Width, m_Height, m_BPP;

	std::ifstream ifile;
	ifile.open(path);
	if (ifile)
		m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);	// Loading the texture
	else
		std::cout << path << " not found!" << std::endl;

	// Generating the texture buffer
	GLCall(glGenTextures(1, &ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, ID));

	// Setting up some basic modes to display texture
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Sending the pixel data to opengl
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

	return { ID, (unsigned int)m_Width, (unsigned int)m_Height };
}

void Renderer::bind_texture(unsigned int id)
{
	r_Data.texture_slots[r_Data.texture_slot_index] = id;
	r_Data.texture_slot_index++;
}


/////////////////////Quads creation/////////////////////


// to render the plane quad with colors
void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (r_Data.index_count >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float texID = 0.0f;
	
	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = { position.x, position.y };
	v2.position = { position.x + size.x, position.y };
	v3.position = { position.x + size.x, position.y + size.y };
	v4.position = { position.x, position.y + size.y };

	v1.texCoords = { 0.0f, 0.0f };
	v2.texCoords = { 1.0f, 0.0f };
	v3.texCoords = { 1.0f, 1.0f };
	v4.texCoords = { 0.0f, 1.0f };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };

	// Pushing vertices into quad buffer
	for (int i = 0; i < 4; i++)
	{
		r_Data.QuadBuffer.push_back(quad[i].position.x);
		r_Data.QuadBuffer.push_back(quad[i].position.y);

		r_Data.QuadBuffer.push_back(quad[i].texCoords.x);
		r_Data.QuadBuffer.push_back(quad[i].texCoords.y);

		r_Data.QuadBuffer.push_back(quad[i].color.r);
		r_Data.QuadBuffer.push_back(quad[i].color.g);
		r_Data.QuadBuffer.push_back(quad[i].color.b);
		r_Data.QuadBuffer.push_back(quad[i].color.a);

		r_Data.QuadBuffer.push_back(quad[i].texID);
	}

	r_Data.index_count += 6;
	renderer_stat.quad_count++;
}

// to render a quad with texture
void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, unsigned int texture)
{
	if (r_Data.index_count >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float texID = 0.0f;
	glm::vec4 color = r_Data.def_color;

	// Checking if the texture already exists. If yes use that else create a new one
	for (int i = 0; i < r_Data.texture_slot_index; i++)
	{
		if (r_Data.texture_slots[i] == texture)
			texID = (float)i;
	}

	if (texID == 0.0f)
	{
		r_Data.texture_slots[r_Data.texture_slot_index] = texture;
		texID = (float)r_Data.texture_slot_index;
		r_Data.texture_slot_index++;
	}

	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = { position.x, position.y };
	v2.position = { position.x + size.x, position.y };
	v3.position = { position.x + size.x, position.y + size.y };
	v4.position = { position.x, position.y + size.y };

	v1.texCoords = { 0.0f, 0.0f };
	v2.texCoords = { 1.0f, 0.0f };
	v3.texCoords = { 1.0f, 1.0f };
	v4.texCoords = { 0.0f, 1.0f };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };

	// Pushing vertices into quad buffer
	for (int i = 0; i < 4; i++)
	{
		r_Data.QuadBuffer.push_back(quad[i].position.x);
		r_Data.QuadBuffer.push_back(quad[i].position.y);

		r_Data.QuadBuffer.push_back(quad[i].texCoords.x);
		r_Data.QuadBuffer.push_back(quad[i].texCoords.y);

		r_Data.QuadBuffer.push_back(quad[i].color.r);
		r_Data.QuadBuffer.push_back(quad[i].color.g);
		r_Data.QuadBuffer.push_back(quad[i].color.b);
		r_Data.QuadBuffer.push_back(quad[i].color.a);

		r_Data.QuadBuffer.push_back(quad[i].texID);
	}

	r_Data.index_count += 6;
	renderer_stat.quad_count++;
}

// to render quad using spritesheets
void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& offset, const std::array<unsigned int, 3>& texture)
{
	if (r_Data.index_count >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float texID = 0.0f;
	glm::vec4 color = r_Data.def_color;

	// Checking if the texture already exists. If yes use that else create a new one
	for (int i = 0; i < r_Data.texture_slot_index; i++)
	{
		if (r_Data.texture_slots[i] == texture[0])
			texID = (float)i;
	}

	if (texID == 0.0f)
	{
		r_Data.texture_slots[r_Data.texture_slot_index] = texture[0];
		texID = (float)r_Data.texture_slot_index;
		r_Data.texture_slot_index++;
	}

	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = { position.x, position.y };
	v2.position = { position.x + size.x, position.y };
	v3.position = { position.x + size.x, position.y + size.y };
	v4.position = { position.x, position.y + size.y };

	v1.texCoords = { (offset.x * offset.z)/texture[1],         (offset.y * offset.w) / texture[2] };
	v2.texCoords = { ((offset.x + 1) * offset.z) / texture[1], (offset.y * offset.w) / texture[2] };
	v3.texCoords = { ((offset.x + 1) * offset.z) / texture[1], ((offset.y + 1) * offset.w) / texture[2] };
	v4.texCoords = { (offset.x * offset.z) / texture[1],       ((offset.y + 1) * offset.w) / texture[2] };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };

	// Pushing vertices into quad buffer
	for (int i = 0; i < 4; i++)
	{
		r_Data.QuadBuffer.push_back(quad[i].position.x);
		r_Data.QuadBuffer.push_back(quad[i].position.y);

		r_Data.QuadBuffer.push_back(quad[i].texCoords.x);
		r_Data.QuadBuffer.push_back(quad[i].texCoords.y);

		r_Data.QuadBuffer.push_back(quad[i].color.r);
		r_Data.QuadBuffer.push_back(quad[i].color.g);
		r_Data.QuadBuffer.push_back(quad[i].color.b);
		r_Data.QuadBuffer.push_back(quad[i].color.a);

		r_Data.QuadBuffer.push_back(quad[i].texID);
	}

	r_Data.index_count += 6;
	renderer_stat.quad_count++;
}


// to render the plane polligon with colors
void Renderer::draw_polligon(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec4& color)
{
	if (r_Data.index_count >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float texID = 0.0f;

	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = p1;
	v2.position = p2;
	v3.position = p3;
	v4.position = p4;

	v1.texCoords = { 0.0f, 0.0f };
	v2.texCoords = { 1.0f, 0.0f };
	v3.texCoords = { 1.0f, 1.0f };
	v4.texCoords = { 0.0f, 1.0f };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };

	// Pushing vertices into quad buffer
	for (int i = 0; i < 4; i++)
	{
		r_Data.QuadBuffer.push_back(quad[i].position.x);
		r_Data.QuadBuffer.push_back(quad[i].position.y);

		r_Data.QuadBuffer.push_back(quad[i].texCoords.x);
		r_Data.QuadBuffer.push_back(quad[i].texCoords.y);

		r_Data.QuadBuffer.push_back(quad[i].color.r);
		r_Data.QuadBuffer.push_back(quad[i].color.g);
		r_Data.QuadBuffer.push_back(quad[i].color.b);
		r_Data.QuadBuffer.push_back(quad[i].color.a);

		r_Data.QuadBuffer.push_back(quad[i].texID);
	}

	r_Data.index_count += 6;
	renderer_stat.quad_count++;
}

// to render the plane polligon with textures
void Renderer::draw_polligon(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, unsigned int texture)
{
	if (r_Data.index_count >= MaxIndexCount)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}

	float texID = 0.0f;
	glm::vec4 color = r_Data.def_color;

	// Checking if the texture already exists. If yes use that else create a new one
	for (int i = 0; i < r_Data.texture_slot_index; i++)
	{
		if (r_Data.texture_slots[i] == texture)
			texID = (float)i;
	}

	if (texID == 0.0f)
	{
		r_Data.texture_slots[r_Data.texture_slot_index] = texture;
		texID = (float)r_Data.texture_slot_index;
		r_Data.texture_slot_index++;
	}

	// Creating vertices of QUAD
	Vertex v1, v2, v3, v4;

	v1.position = p1;
	v2.position = p2;
	v3.position = p3;
	v4.position = p4;

	v1.texCoords = { 0.0f, 0.0f };
	v2.texCoords = { 1.0f, 0.0f };
	v3.texCoords = { 1.0f, 1.0f };
	v4.texCoords = { 0.0f, 1.0f };

	v1.color = color;
	v2.color = color;
	v3.color = color;
	v4.color = color;

	v1.texID = texID;
	v2.texID = texID;
	v3.texID = texID;
	v4.texID = texID;

	Vertex quad[] = { v1, v2, v3, v4 };

	// Pushing vertices into quad buffer
	for (int i = 0; i < 4; i++)
	{
		r_Data.QuadBuffer.push_back(quad[i].position.x);
		r_Data.QuadBuffer.push_back(quad[i].position.y);

		r_Data.QuadBuffer.push_back(quad[i].texCoords.x);
		r_Data.QuadBuffer.push_back(quad[i].texCoords.y);

		r_Data.QuadBuffer.push_back(quad[i].color.r);
		r_Data.QuadBuffer.push_back(quad[i].color.g);
		r_Data.QuadBuffer.push_back(quad[i].color.b);
		r_Data.QuadBuffer.push_back(quad[i].color.a);

		r_Data.QuadBuffer.push_back(quad[i].texID);
	}

	r_Data.index_count += 6;
	renderer_stat.quad_count++;
}


/////////////////////Some Render statistics thingy/////////////////////

void Renderer::ResetStats()
{
	renderer_stat.draw_count = 0;
	renderer_stat.quad_count = 0;
}

Renderer::Stat Renderer::GetStats()
{
	return renderer_stat;
}
