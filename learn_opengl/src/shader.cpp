#include "shader.h"
#include "renderer.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



Shader::Shader(const std::string& shaderPath)
	: m_FilePath(shaderPath), m_RenderID(0)
{
	ShaderProgramSource source = ParseShader(shaderPath);
	m_RenderID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RenderID);
}

ShaderProgramSource Shader::ParseShader(const std::string& shaderPath)
{
	std::ifstream stream(shaderPath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream shaderStack[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			shaderStack[(int)type] << line << '\n';
		}
	}

	return { shaderStack[0].str(), shaderStack[1].str() };
}

void Shader::Bind() const
{
	glUseProgram(m_RenderID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

int Shader::checkCompileErrors(unsigned int type, unsigned int shader)
{
	int success;
	char infoLog[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		glDeleteShader(shader);
		return 0;
	}
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& src)
{
	unsigned int id = glCreateShader(type);
	const char* source = src.c_str();
	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	checkCompileErrors(type, id);

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Attach files and link
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//glDetachShader(vs);
	//glDetachShader(fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform3fv(const std::string& name, const glm::vec3& vector)
{
	glUniform3fv(GetUniformLocation(name), 1, &vector[0]);
}

void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]/*glm::value_ptr(matrix)*/);
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	int location = glGetUniformLocation(m_RenderID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning:  uniform '" << name << "' doesn't exist!" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}

