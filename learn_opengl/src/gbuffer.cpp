#include "gbuffer.h"
#include "renderer.h"
#include <vector>
#include <stdexcept>
 
gBuffer::gBuffer(unsigned int m_Width, unsigned int m_Height)
    :
    m_Width(m_Width),
    m_Height(m_Height),
    m_gPosition(0),
    m_gNormal(0),
    m_gAlbedoSpec(0)
{
    if (m_Width <= 0 || m_Height <= 0) {
        throw std::invalid_argument("Framebuffer's size must be greater than zero");
    }
	glGenFramebuffers(1, &m_RenderID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    std::vector<GLenum> colorAttachments{
        GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
        GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7,
        GL_COLOR_ATTACHMENT8, GL_COLOR_ATTACHMENT9, GL_COLOR_ATTACHMENT10, GL_COLOR_ATTACHMENT11,
        GL_COLOR_ATTACHMENT12, GL_COLOR_ATTACHMENT13, GL_COLOR_ATTACHMENT14, GL_COLOR_ATTACHMENT15
    };


    attachTextureToColorAttachment();    
    specifyBuffer();
    initDepthRenderBuffer(); // add depth buffer to bound FrameBuffer
    //generateDepthMap(); // attach depth map texture to bound FrameBuffer

}

gBuffer::~gBuffer()
{
	glDeleteFramebuffers(1, &m_RenderID);
}

bool gBuffer::isBufferComplete()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

void gBuffer::attachTexture(const Texture &texture)
{
    
}

void gBuffer::attachTextureToColorAttachment()
{
    // position color buffer
    glGenTextures(1, &m_gPosition);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition, 0);

    // normal color buffer
    glGenTextures(1, &m_gNormal);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal, 0);

    // - color + specular color buffer
    glGenTextures(1, &m_gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gAlbedoSpec, 0);

    // - Bright Color Thresholded
    glGenTextures(1, &m_BrightColor);
    glBindTexture(GL_TEXTURE_2D, m_BrightColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_BrightColor, 0);

    // - Bright Color Thresholded
    glGenTextures(1, &m_Metallic);
    glBindTexture(GL_TEXTURE_2D, m_Metallic);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_Metallic, 0);

    // - Bright Color Thresholded
    glGenTextures(1, &m_Roughness);
    glBindTexture(GL_TEXTURE_2D, m_Roughness);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, m_Roughness, 0);

    // - Bright Color Thresholded
    glGenTextures(1, &m_AO);
    glBindTexture(GL_TEXTURE_2D, m_AO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, m_AO, 0);

    

}

void gBuffer::attachTextureToDepthAttachment()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gDepth);

}

void gBuffer::initDepthRenderBuffer()
{
    // - Create & attach depth buffer
    glGenRenderbuffers(1, &m_gDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_gDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_gDepth);
}

void gBuffer::generateDepthMap()
{
    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    // Tell opengl we aren't going to render any color data
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

}

void gBuffer::specifyBuffer()
{
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
    unsigned int attachments[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6};
    glDrawBuffers(7, attachments);
}

void gBuffer::readbuffer()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RenderID);
}

void gBuffer::writetodefault()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void gBuffer::activate()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gPosition);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gNormal);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_gAlbedoSpec);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_BrightColor);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, m_Metallic);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, m_Roughness);

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, m_AO);

}

void gBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
}

void gBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
