#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include "stb_image/stb_image.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class radianceImage
{
public:
	bool flipvertically;
	radianceImage(std::string const& path, bool flip = true) : flipvertically(flip)
	{
		stbi_set_flip_vertically_on_load(flip);
		loadMap(path);
	}

	void setActive(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
	}
private:
	unsigned int hdrTexture;

	void loadMap(std::string const& path)
	{
		int m_width, m_height, m_nrComponents;
		float* m_localBuffer = stbi_loadf(path.c_str(), &m_width, &m_height, &m_nrComponents, 0);

		if (m_localBuffer)
		{
			glGenTextures(1, &hdrTexture);
			glBindTexture(GL_TEXTURE_2D, hdrTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, m_localBuffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(m_localBuffer);
		}
		else
		{
			std::cout << "Failed to load HDR radiance image." << std::endl;
		}
	}
};