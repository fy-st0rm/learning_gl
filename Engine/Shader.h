#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"


class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
	std::unordered_map<std::string, int> m_LocationCache;

public:
	Shader(const std::string& VertexShader, const std::string& FragmentShader);
	Shader();
	~Shader();

	void create(const std::string& VertexShader, const std::string& FragmentShader);
	void Destroy();
	void change(const std::string& VertexShader, const std::string& FragmentShader);

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1iv(const std::string& name, int size, int* value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformVec3f(const std::string& name, const glm::vec3& vector);

private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};