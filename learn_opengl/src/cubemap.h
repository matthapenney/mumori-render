#pragma once

#include "stb_image/stb_image.h"
#include "shader.h"
#include "renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>

class Cubemap
{
public:
	Cubemap();
	~Cubemap();

	void SetParams();
	void AddFace(std::vector<std::string>& faces);
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	void PushUniforms(Shader& shader, int textureUnit, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPosition, glm::vec3 lightPosition, glm::vec3 lightColor, float g);

	// Helpers
	inline int GetWidth() const { return c_Width; }
	inline int GetHeight() const { return c_Height; }

private:;
	unsigned int m_CubemapID;
	unsigned char* m_CubeBuffer;
	int c_Width, c_Height, c_nrChannels;
};