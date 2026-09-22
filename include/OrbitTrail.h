#pragma once 

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "shaderClass.h" 
#include "Camera.h"

class OrbitTrail {
public:
    // Pass maxPoints = 0 for unlimited length, or any positive number for a capped trail
    OrbitTrail(size_t maxPoints = 0, glm::vec3 trailColor = glm::vec3(1.0f, 1.0f, 1.0f));

    void updateTrail(glm::vec3 currentPos);
    void drawTrail(Shader& shader, Camera& camera);
    void drawTrail(Shader& shader);
    void clear();

    // Setters to change limits or colors dynamically at runtime
    void setMaxPoints(size_t newMaxPoints);
    void setColor(glm::vec3 newColor) { color = newColor; }

private:
    size_t maxPoints; // when 0, infinite trail 
    glm::vec3 color;  // Declare color SECOND

    std::vector<glm::vec3> positions;
    VAO VAO;
    VBO VBO;
};
