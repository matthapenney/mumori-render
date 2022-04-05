#include "framebuffer.h"
#include "renderer.h"

// Some kind of create() function
// glGenFramebuffers(1, &m_RenderID);
// glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
// 

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &m_RenderID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID); 
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_RenderID);
}

bool Framebuffer::isBufferComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
		//Unbind();
		return true;
	}
	else
	{
		//Unbind();
		return false;
	}
}

void Framebuffer::AttachTexture(FramebufferSpecification FS, TextureAttachSpecification TAS)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	if (FS.containsColor)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, TAS.m_Width, TAS.m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
	}

	if (FS.containsDepth && FS.containsStencil)
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, TAS.m_Width, TAS.m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_TextureID, TAS.mipLevel);
	}		
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, TAS.mipLevel);
}

void Framebuffer::AttachRBO(FramebufferSpecification FS)
{
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, FS.m_Width, FS.m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
}

void Framebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::TextureBind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Framebuffer::DeleteObject()
{
	glDeleteFramebuffers(1, &m_RenderID);
}
