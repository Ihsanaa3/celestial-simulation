#pragma once
#include <iostream> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <cstdlib>
#include <ctime>  
#include <cmath> 

namespace rd{
    void randseed();
    int ransign();
    int randIntSign(int ub);
    float randsignf();
    float randf();
    float randfb(float upper_bound);
}

namespace et{ 
    GLFWwindow* initialiseGLFW(int width, int height, const char* title); 
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window);
    void versioncheck(); 
}