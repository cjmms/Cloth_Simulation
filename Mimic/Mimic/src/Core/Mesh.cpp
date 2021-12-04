
#include "Mesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <iostream>




Mesh::Mesh(std::vector<Vertex> vertices,
	std::vector<Texture> textures,
	std::vector<unsigned int> indices)
	: vertices(vertices), textures(textures), indices(indices)

{
	setup();
}


// !!!!!  If there are multiple textures with same type(ex. texture_albedo)
// !!!!!  the LAST one that get iterated will be used

// Try to avoid having multiple textures with same type

// Why I designed this way:
// Becasue I don't understand why the fuck you need 2 albedo textures for 1 fucking PBR mesh

// uniform name: material.texture_diffuse   (texture_diffuse is type name in this case)
void Mesh::Draw(Shader& shader) const
{
    // setup texture uniform
    for (unsigned int i = 0; i < textures.size(); i++)
        shader.setTexture(("material." + textures[i].type).c_str(), textures[i].id);
    
    // draw mesh
    shader.Bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    shader.unBind();
}

void Mesh::setup()
{
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    VertexBuffer vbo(&vertices[0], vertices.size() * sizeof(Vertex));

    // IBO
    IndexBuffer ibo(&indices[0], indices.size());

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

    // vertex tex
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    // vertex texture coords
    //glEnableVertexAttribArray(2);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));



    glBindVertexArray(0);
}

