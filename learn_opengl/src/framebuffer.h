#pragma once

struct FramebufferSpecification
{
	unsigned int Samples = 1;
	bool containsDepth = false;
	bool containsStencil = false;
	bool containsColor = true;
	const unsigned int m_Width = 1920;
	const unsigned int m_Height = 1080;

};

struct TextureAttachSpecification
{
	const unsigned int m_Width = 1920;
	const unsigned int m_Height = 1080;
	unsigned int kColors = 0;
	unsigned int mipLevel = 0;
};

class Framebuffer
{
private:
	unsigned int m_RenderID;
	unsigned int m_TextureID;
	unsigned int m_RBO;
	

public:
	Framebuffer();
	~Framebuffer();

	TextureAttachSpecification TAS;
	FramebufferSpecification FS;
	//void Validate();
	bool isBufferComplete();
	void AttachTexture(FramebufferSpecification FS, TextureAttachSpecification TAS);
	void AttachRBO(FramebufferSpecification FS);
	void Bind() const;
	void Unbind() const;
	void TextureBind() const;
	// need?
	void DeleteObject();
};