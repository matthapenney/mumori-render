#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	:
	vertices(vertices), indices(indices), textures(textures) 
{

	setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::Draw(Shader& shader) const
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	IndexBuffer IB(&indices[0], indices.size());

	for (unsigned int i = 0; i < textures.size(); i++)
	{	
		std::string number;
		std::string name = textures[i].GetType();
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		shader.Bind();
		shader.SetUniform1f(("material." + name + number).c_str(), i);
		textures[i].Bind(i);

	}

	textures[0].Bind(0);
	VA.Bind();
	IB.Bind();
	glDrawElements(GL_TRIANGLES, IB.GetCount(), GL_UNSIGNED_INT, nullptr);
}


void Mesh::setupMesh()
{
	VertexBuffer VB(&vertices[0], vertices.size() * sizeof(Vertex));
	//IndexBuffer IB(&indices[0], indices.size());
	layout.Push<float>(vertices[0].Position.length());
	layout.Push<float>(vertices[0].Normal.length());
	layout.Push<float>(vertices[0].TexCoords.length());
	VA.AddBuffer(VB, layout);

}
