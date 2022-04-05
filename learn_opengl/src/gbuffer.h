#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "texture.h"


class gBuffer 
{
public:
	gBuffer(unsigned int m_Width, unsigned int m_Height);
	~gBuffer();

	bool isBufferComplete();
	void attachTexture(const Texture &texture);
	void attachTextureToColorAttachment();
	void attachTextureToDepthAttachment();
	void initDepthRenderBuffer();
	void generateDepthMap();
	void specifyBuffer();
	
	void readbuffer();
	void writetodefault();
	void activate();
	void Bind();
	void Unbind();

	// Screen Params 
	unsigned int m_Width;
	unsigned int m_Height;

private:
	unsigned int m_RenderID;
	unsigned int m_gDepth;

	// Color Buffers
	unsigned int m_gPosition;
	unsigned int m_gNormal;
	unsigned int m_gAlbedoSpec;
	unsigned int m_BrightColor;
	unsigned int m_depthMap;
	unsigned int m_Roughness;
	unsigned int m_Metallic;
	unsigned int m_AO;
	


};