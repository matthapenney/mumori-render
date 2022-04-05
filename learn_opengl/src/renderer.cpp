#include "renderer.h"
#include <iostream>

void GLCLearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& VA, /*const IndexBuffer& IB,*/ const Shader& shader) const
{
	shader.Bind();
	VA.Bind();
	//IB.Bind();
	//glDrawElements(GL_TRIANGLES, NULL, GL_UNSIGNED_INT, nullptr);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}
