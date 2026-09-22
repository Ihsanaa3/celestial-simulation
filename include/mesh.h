#pragma once 

#include<string> 

#include"VAO.h" 
#include"EBO.h" 
#include"texture.h" 
#include"camera.h" 

class Mesh {
    
public:
	std::vector <Vertex> vertices4A; 
	std::vector <VertexPC> verticesPC; 
	std::vector <VertexNPC> verticesNPC; 

	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// Store VAO in public so it can be used in the Draw function
	VAO VAO; 

	// Initializes the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// Initialises the mesh for the grid stuff 
	Mesh(std::vector <VertexPC>& vertices, std::vector <GLuint>& indices);   

	// Initialises the mesh with normal, pos and col 
	Mesh(std::vector <VertexNPC>& vertices, std::vector <GLuint>& indices); 

	// Draws the mesh
	void Draw(Shader& shader, GLenum primitiveMode);
	void Draw(Shader& shader, Camera& camera);  
	void Draw(Shader& shader, Camera& camera, GLenum primitiveMode); 
};