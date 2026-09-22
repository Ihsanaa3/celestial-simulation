#include"rotation.h"

const float PI = 3.1415926; 

std::vector<float> rtt::rotate(float radians, float x, float y){ 
    float rx =  (x * cos(radians)) + (y * -sin(radians)); 
    float ry =  (x * sin(radians)) + (y * cos(radians)); 
    // Snap to 0 if the value is microscopic
    if (std::abs(rx) < 1e-6) rx = 0.0f;
    if (std::abs(ry) < 1e-6) ry = 0.0f;

    return {rx, ry}; 
}

float rtt::angle_between_vertices(int number_of_vertices)
{
    // radians 
    float angle = (2 * PI) / number_of_vertices; 
    return angle;
}

std::vector<float> rtt::vertex_coordinates(int NOV, float start_x, float start_y, float R, float G, float B){
    float angle = angle_between_vertices(NOV); 
    std::vector<float> vertex; 
    std::vector<float> newVertex;
    std::vector<float> solidColour = {R, G, B}; 
    for(int i = 0; i < NOV ; i++){ 
        newVertex = rotate(i * angle, start_x, start_y); 
        newVertex.push_back(0.0f); 
        vertex.insert(vertex.end(), newVertex.begin(), newVertex.end()); 
        vertex.insert(vertex.end(), solidColour.begin(), solidColour.end()); 
    }; 
    return vertex; 
}
