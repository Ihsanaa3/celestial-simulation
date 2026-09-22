#include "OrbitTrail.h"

OrbitTrail::OrbitTrail(size_t maxPoints, glm::vec3 trailColor) 
    : maxPoints(maxPoints),
      color(trailColor),
      VBO((maxPoints > 0 ? maxPoints : 500) * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW)
{ 
    if (maxPoints > 0) {
        positions.reserve(maxPoints);
    } else {
        positions.reserve(500); // Initial capacity hint for dynamic growth
    }

    VAO.Bind(); 
    VBO.Bind();

    // Link position attribute (Location 0)
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);

    VAO.Unbind();
    VBO.Unbind();
}

void OrbitTrail::updateTrail(glm::vec3 currentPos) {
    positions.push_back(currentPos); 

    // If a limit is set and exceeded, pop the oldest position from the front
    if (maxPoints > 0 && positions.size() > maxPoints) {
        positions.erase(positions.begin());
    }

    VBO.Bind(); 
    // Reallocate/stream buffer data to GPU
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
    VBO.Unbind(); 
}

void OrbitTrail::drawTrail(Shader& shader, Camera& camera) {
    if (positions.size() < 2) return; 

    shader.Activate(); 
    VAO.Bind(); 

    glm::mat4 model = glm::mat4(1.0f); 

    camera.Matrix(shader, "camMatrix");
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader.ID, "trailColor"), color.r, color.g, color.b); 

    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(positions.size())); 

    VAO.Unbind();
}

void OrbitTrail::drawTrail(Shader& shader) {
    if (positions.size() < 2) return; 

    shader.Activate(); 
    VAO.Bind(); 

    glm::mat4 model = glm::mat4(1.0f); 

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(glGetUniformLocation(shader.ID, "trailColor"), color.r, color.g, color.b); 

    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(positions.size())); 

    VAO.Unbind();
}

void OrbitTrail::setMaxPoints(size_t newMaxPoints) {
    maxPoints = newMaxPoints;
    
    // Trim current buffer immediately if it exceeds the new limit
    if (maxPoints > 0 && positions.size() > maxPoints) {
        positions.erase(positions.begin(), positions.begin() + (positions.size() - maxPoints));
    }
}

void OrbitTrail::clear() {
    positions.clear();
}