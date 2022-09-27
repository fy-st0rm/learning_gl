#include "Shader.h"
#include "ErrorHandler.h"

#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader()
    :m_RendererID(0)
{

}

Shader::Shader(const std::string& VertexShader, const std::string& FragmentShader)
    :m_RendererID(0)
{
    //ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(VertexShader, FragmentShader);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::create(const std::string& VertexShader, const std::string& FragmentShader)
{
    m_RendererID = CreateShader(VertexShader, FragmentShader);   
}

void Shader::change(const std::string& VertexShader, const std::string& FragmentShader)
{
    Destroy();
    m_RendererID = CreateShader(VertexShader, FragmentShader);   
}

void Shader::Destroy()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1iv(const std::string& name, int size, int* value)
{
    GLCall(glUniform1iv(GetUniformLocation(name), size, value));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformVec3f(const std::string& name, const glm::vec3& vector)
{
    GLCall(glUniform3f(GetUniformLocation(name), vector.x, vector.y, vector.z));
}

int Shader::GetUniformLocation(const std::string& name)
{
    // Searching if the location already exists in the cache
    if (m_LocationCache.find(name) != m_LocationCache.end())
        return m_LocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Uniform " << name << " doesn't exist!!" << std::endl;
    else
        m_LocationCache[name] = location;

    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& sourceFile)
{
    std::stringstream shaderSrc;

    // Reading the file
    std::ifstream stream(sourceFile);
    std::string line;
    while (std::getline(stream, line))
    {
        shaderSrc << line << "\n";
    }

    std::string shader = shaderSrc.str();

    GLCall(unsigned int id = glCreateShader(type)); // Creating the shader of the required type
    const char* src = shader.c_str();               // Converting the source into const char*
    GLCall(glShaderSource(id, 1, &src, nullptr));   // Providing our shader source code
    GLCall(glCompileShader(id));                    // Compiling the shader

    // Error handleling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));  // Getting the result of the compilation of shader
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length)); // Getting the length of the error
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        // Printing out the error msg
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());    // Creating our shader program

    // Compiling our shaders
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attaching the shaders into one
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // Deleting the shader
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
