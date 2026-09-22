#pragma once 

#include<cmath> 
#include<vector>
#include<iostream> 


namespace rtt{
    
    std::vector<float> rotate(float radians, float x, float y); 
    float angle_between_vertices(int number_of_vertices); 
    std::vector<float> vertex_coordinates(int NOV, float start_x, float start_y, float R, float G, float B); 

}