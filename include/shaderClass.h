#pragma once 
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <string>
#include <glad/glad.h>

class Shader {
public:
    GLuint ID;
    
    // Updated constructor declaration
    Shader(const std::string& vertexFile, const std::string& fragmentFile);

    void Activate();
    void Delete();

private:
    void compileErrors(unsigned int shader, const char* type);
};