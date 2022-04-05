#include "vertexarray.h"
#include "renderer.h"
#include "vertexbufferlayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RenderID);

	// color attrib with updated stride (24 bytes) and offset
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RenderID);
}

void VertexArray::AddBuffer(const VertexBuffer& VB, const VertexBufferLayout& layout)
{
	// Bind Array
	Bind();
	// Bind Buffer
	VB.Bind();

	// Set up layout 
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.isNormalized, layout.GetStride(), (const void*)offset);
		offset += element.count* VertexBufferElement::getSize(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RenderID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
