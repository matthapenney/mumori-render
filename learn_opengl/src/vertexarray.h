#pragma once

#include "vertexbuffer.h"

class VertexBufferLayout;

class VertexArray 
{
private:
	unsigned int m_RenderID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& VB, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};