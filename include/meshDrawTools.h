#pragma once 
#include"mesh.h" 

const float PI = 3.14159265359f; 

namespace drt{ 

    inline glm::vec2 rotated(float rotationInDegrees, float xValue, float yValue) {
        float r = glm::radians(rotationInDegrees); // GLM provides built-in degree-to-radian conversion
        
        float xcoor = xValue * cos(r) - yValue * sin(r); 
        float ycoor = xValue * sin(r) + yValue * cos(r); 
        
        return glm::vec2(xcoor, ycoor);  
    }


    inline Mesh createSphere(float radius = 1.0f, int sector = 20, int stack = 20, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f)){
        std::vector<GLuint> sphereIndices; 
        std::vector<VertexNPC> sphereVertices;

        // sector is the Longitude steps (around) = phi 
        // stack is the Latitude steps (up/down) = theta 

        for (int i = 0; i <= stack; ++i){ 
            // Latitude angle theta from 0 to PI
            float theta = (PI / 2.0f) - (((float)i / stack) * PI); 
            float xy = radius * cosf(theta); // Radius of the current ring
            float y = radius * sinf(theta); 

            for (int j = 0; j <= sector; ++j){ 
                // Longitude angle phi from 0 to 2PI
                float phi = ((float)j / sector) * 2.0f * PI; 

                // Position 
                float x = xy * cosf(phi); 
                float z = xy * sinf(phi); 
                glm::vec3 pos(x, y, z); 

                // For a sphere centered at (0,0,0), the Normal is just the normalized position! as the vector of the radius is normal to the sphere face 
                glm::vec3 normal = glm::normalize(pos); 

                sphereVertices.push_back(VertexNPC{pos, colour, normal}); 
            }
        } 

        for (int i = 0; i < stack; ++i){
            int k1 = i * (sector + 1); // Current stack
            int k2 = k1 + sector + 1;  // Next stack

            for (int j = 0; j < sector; ++j, ++k1, ++k2){
                // Two triangles per quad ball cell (except at top and bottom poles) 
                if (i != 0){
                    sphereIndices.push_back(k1); 
                    sphereIndices.push_back(k2); 
                    sphereIndices.push_back(k1 + 1);  
                }if ( i != (stack - 1)){
                    sphereIndices.push_back(k1 + 1); 
                    sphereIndices.push_back(k2); 
                    sphereIndices.push_back(k2 + 1); 
                }
            }
        } 
        return Mesh(sphereVertices, sphereIndices);
    }; 

    inline Mesh createAxis(float scale = 1.0f, glm::vec3 colour1 = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 colour2 = glm::vec3(0.0f, 0.0f, 0.0f)){

        VertexPC vertices[] = {
        glm::vec3( scale,   0.0f,   0.0f), colour1, 
        glm::vec3(-scale,   0.0f,   0.0f), colour2, 
        glm::vec3(  0.0f,  scale,   0.0f), colour1, 
        glm::vec3(  0.0f, -scale,   0.0f), colour2, 
        glm::vec3(  0.0f,   0.0f,  scale), colour1, 
        glm::vec3(  0.0f,   0.0f, -scale), colour2, 
        };

        GLuint indices[] = {
        0, 1, 2, 3, 4, 5
        }; 

        std::vector <VertexPC> verts(vertices, vertices + sizeof(vertices) / sizeof(VertexPC));
        std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));

        return Mesh(verts, ind); 
    }; 

    inline Mesh createGrid(float gridLength, int noGrids, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f), bool xyPlane = true, bool xzPlane = true, bool yzPlane = true) {
        std::vector<GLuint> indices; 
        std::vector<VertexPC> vertices; 

        if (xyPlane) {
            float z = 0.0f; 
            for (int i = -noGrids; i <= noGrids; ++i) { 
                vertices.push_back(VertexPC{ glm::vec3(i * gridLength, noGrids * gridLength, z), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(i * gridLength, -noGrids * gridLength, z), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(noGrids * gridLength, i * gridLength, z), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(-noGrids * gridLength, i * gridLength, z), colour }); 
            }
        }
        if (xzPlane) {
            float y = 0.0f; 
            for (int i = -noGrids; i <= noGrids; ++i) { 
                vertices.push_back(VertexPC{ glm::vec3(i * gridLength, y, noGrids * gridLength), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(i * gridLength, y, -noGrids * gridLength), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(noGrids * gridLength, y, i * gridLength), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(-noGrids * gridLength, y, i * gridLength), colour }); 
            }
        }
        if (yzPlane) {
            float x = 0.0f; 
            for (int i = -noGrids; i <= noGrids; ++i) { 
                vertices.push_back(VertexPC{ glm::vec3(x, i * gridLength, noGrids * gridLength), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(x, i * gridLength, -noGrids * gridLength), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(x, noGrids * gridLength, i * gridLength), colour }); 
                vertices.push_back(VertexPC{ glm::vec3(x, -noGrids * gridLength, i * gridLength), colour }); 
            }
        }

        // Generate indices 1:1 matching the lines array
        for (size_t k = 0; k < vertices.size(); ++k) {
            indices.push_back(static_cast<GLuint>(k)); 
        }

        return Mesh(vertices, indices); 
    }; 

    inline Mesh createCircle(float radius, int resolution, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f)){
        if(resolution < 3){
            std::cout << "resolution is less than 3, triangle will be outputted. " << std::endl; 
            resolution = 3; 
        }

        std::vector<GLuint> circleIndices; 
        std::vector<VertexPC> circleVertices; 

        circleVertices.push_back(VertexPC{glm::vec3(0.0f, 0.0f, 0.0f), colour});
     
        float z = 0; 
        float nx, ny; 
        float angle; 
        for (int i = 0; i < resolution; ++i)
        {   
            angle = i * 2 * PI / resolution; 
            nx = radius * cosf(angle); 
            ny = -radius * sin(angle); 
            circleVertices.push_back(VertexPC{glm::vec3(nx, ny, z), colour}); 
        } 
        for (int i = 0; i < resolution; ++i)
        {
            if(i + 2 > resolution)
            {
                circleIndices.push_back(0);
                circleIndices.push_back(i + 1); 
                circleIndices.push_back(1);  
            }else
            {
                circleIndices.push_back(0); 
                circleIndices.push_back(i + 1); 
                circleIndices.push_back(i + 2); 
            }
        } 
        
        return Mesh(circleVertices,circleIndices); 
    }; 

    inline Mesh createSquare(float sideLength, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f)){ 
        float halfLength = sideLength / 2; 
        VertexPC vertices[] = {
        glm::vec3( halfLength,  halfLength, 0.0f), colour,  
        glm::vec3( halfLength, -halfLength, 0.0f), colour,  
        glm::vec3(-halfLength,  halfLength, 0.0f), colour,  
        glm::vec3(-halfLength, -halfLength, 0.0f), colour,  
        };

        GLuint indices[] = {
        0, 1, 2, 
        1, 2, 3
        }; 

        std::vector <VertexPC> verts(vertices, vertices + sizeof(vertices) / sizeof(VertexPC));
        std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));

        return Mesh(verts, ind);         
    };

    inline Mesh createTriangle(float sideLength, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f)){ 
        float x = sideLength / sqrt(3); 

        VertexPC vertices[] = {
        glm::vec3(drt::rotated(0.0f, 0.0f, x) , 0.0f), colour,  
        glm::vec3(drt::rotated(120.0f, 0.0f, x), 0.0f), colour,  
        glm::vec3(drt::rotated(240.0f, 0.0f, x), 0.0f), colour,  
        };

        GLuint indices[] = {
        0, 1, 2, 
        }; 

        std::vector <VertexPC> verts(vertices, vertices + sizeof(vertices) / sizeof(VertexPC));
        std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));

        return Mesh(verts, ind);         
    };
} 