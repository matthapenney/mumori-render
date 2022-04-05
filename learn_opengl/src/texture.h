#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& path, const std::string& type);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	
	// Helpers
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height;}
	inline std::string GetType() const { return m_type; }
private:
	std::string m_FilePath;
	unsigned int m_RenderID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BitsPP;
	std::string m_type;
};