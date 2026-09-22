#include"VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// second constructor for mesh 
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
} 

// third constructor for grid stuff 
VBO::VBO(std::vector<VertexPC>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPC), vertices.data(), GL_STATIC_DRAW);
} 

// fourth constructor for normal added to col and pos 
VBO::VBO(std::vector<VertexNPC>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexNPC), vertices.data(), GL_STATIC_DRAW);
} 

// fifth constructor for trails 
VBO::VBO(GLsizeiptr maxpoints, const void* nullpointer, GLenum primitiveDrawType){
	glGenBuffers(1, &ID); 
	glBindBuffer(GL_ARRAY_BUFFER, ID); 
	glBufferData(GL_ARRAY_BUFFER, maxpoints, nullpointer, primitiveDrawType);
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}