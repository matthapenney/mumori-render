#include "cubemap.h"




Cubemap::Cubemap()
	:
	c_Width(0), c_Height(0), c_nrChannels(0), m_CubeBuffer(nullptr)
{
	// Create Cubemap -> Draft before factoring into api
	glGenTextures(1, &m_CubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapID);


	//for (unsigned int i = 0; i < 6; i++)
	//{
	//	m_CubeBuffer = stbi_load(path.c_str(), &c_Width, &c_Height, &c_nrChannels, 0);
	//	if (m_CubeBuffer)
	//	{
	//		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, c_Width, c_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_CubeBuffer);
	//		stbi_image_free(m_CubeBuffer);
	//	}
	//	std::cout << "Cubemap texture failed to load at path: " << path << std::endl;
	//	stbi_image_free(m_CubeBuffer);
	//}
	

}
Cubemap::~Cubemap()
{
}
;

void Cubemap::SetParams()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void Cubemap::AddFace(std::vector<std::string>& faces)
{
	for (unsigned int i = 0; i < 6; i++)
	{
		m_CubeBuffer = stbi_load(faces[i].c_str(), &c_Width, &c_Height, &c_nrChannels, 0);
		if (m_CubeBuffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, c_Width, c_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_CubeBuffer);
			stbi_image_free(m_CubeBuffer);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at face: " << faces[i] << std::endl;
			stbi_image_free(m_CubeBuffer);
		}
	}

}


void Cubemap::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapID);
}

void Cubemap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::PushUniforms(Shader &shader, int textureUnit, glm::mat4 view, glm::mat4 projection, glm::vec3 viewPosition, glm::vec3 lightPosition, glm::vec3 lightColor, float g)
{
	shader.SetUniform1i("environmentMap", textureUnit);
	shader.SetUniform3fv("sunColor", lightColor);
	shader.SetUniform3fv("viewPosition", viewPosition);
	shader.SetUniform3fv("lightPosition", lightPosition);
	shader.SetUniformMatrix4fv("view", view);
	shader.SetUniformMatrix4fv("projection", projection);
	shader.SetUniform1f("g", g);
}
