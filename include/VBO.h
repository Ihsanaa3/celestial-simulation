#pragma once 

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector> 

struct Vertex{
	glm::vec3 position; 
	glm::vec3 colour; 
	glm::vec3 normal; 
	glm::vec2 texUV; 
}; 

struct VertexPC{
	glm::vec3 position; 
	glm::vec3 colour; 
}; 

struct VertexNPC{
	glm::vec3 position; 
	glm::vec3 colour; 
	glm::vec3 normal; 
};

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	
	// Constructor that generates a Vertex Buffer Object and links it to vertices 
	VBO(GLfloat* vertices, GLsizeiptr size); 

	// Second constructor for mesh
	VBO(std::vector<Vertex>& vertices);

	// third constructor for grid stuff 
	VBO(std::vector<VertexPC>& vertices); 

	// fourth constructor for normal added to col and pos 
	VBO(std::vector<VertexNPC>& vertices); 

	// fifth constructor for trails 
	VBO(GLsizeiptr maxpoints, const void* nullpointer, GLenum primitiveDrawType); 

	// Binds the VBO
	void Bind(); 

	// Unbinds the VBO
	void Unbind();

	// Deletes the VBO
	void Delete();
};
