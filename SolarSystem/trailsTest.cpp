#include"mesh.h" 
#include"extraFunctions.h" 
#include"OrbitTrail.h"
// Define Pi if not available
const float PI = 3.14159265359f;  

Mesh createSphere(float radius, int sector, int stack, glm::vec3 colour){
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

Mesh createAxis(glm::vec3 colour1, glm::vec3 colour2, float scale){

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

Mesh createGrid(float gridLength, int noGrids, glm::vec3 colour, bool xyPlane, bool xzPlane, bool yzPlane) {
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

const unsigned int width = 800;
const unsigned int height = 800; 

const glm::mat4 iden4 = glm::mat4(1.0f); 

int main(){
    GLFWwindow* window = et::initialiseGLFW(width, height, "Window" ); 

    Mesh ball = createSphere(1.0f, 20, 20, glm::vec3(0.12f, 0.56f, 1.00f)); 
    Mesh axis = createAxis(glm::vec3(0.0, 0.5, 1.0), glm::vec3(0.0, 1.0, 0.5), 20); 
    Mesh grid = createGrid(1.0f, 10, glm::vec3(0.0, 0.5, 1.0), true, false, false);  

    OrbitTrail ballTrail(200);  

    rd::randseed(); 

    Shader defaultShader("C:/OGL/res/vertex/udef.vert.txt", "C:/OGL/res/fragment/default.frag.txt");   
    Shader trailShader("C:/OGL/res/vertex/udef.vert.txt", "C:/OGL/res/fragment/trail.frag.txt"); 
    glEnable(GL_DEPTH_TEST); 

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 50.0f));

    double prevTime = glfwGetTime(); 
    float angle = 0.0f; 
 
    while (!glfwWindowShouldClose(window))
	{
		// Specify the  of the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double crntTime = glfwGetTime(); 
        
        if(crntTime - prevTime >= 1.0f / 60.0f){
            angle += PI / 24; 
            prevTime = crntTime; 
        }

		camera.updateMatrix(45.0f, 0.1f, 100.0f); 

        camera.Inputs(window);     

        defaultShader.Activate(); 

        glm::mat4 ballTranslate = glm::translate(iden4 , glm::vec3(10.0f, 1.0f, 0.0f)); 
		glm::mat4 ballRot = glm::rotate(iden4, angle, glm::vec3(sinf(angle), cosf(angle), 1.0f)); 
        glm::mat4 ballModel = ballRot * ballTranslate; 

        glm::vec3 ballPos = glm::vec3(ballModel[3]);  
        ballTrail.updateTrail(ballPos); 

        // glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(ballModel));

        // ball.Draw(defaultShader, camera, GL_TRIANGLES);  

        glm::mat4 AxisModel = iden4; 

        glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(AxisModel));
        
        // axis.Draw(defaultShader, camera, GL_LINES);  
        
        grid.Draw(defaultShader, camera, GL_LINES); 

        ballTrail.drawTrail(trailShader, camera); 

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
        // lets you esc out 
		et::processInput(window); 
		// Take care of all GLFW events
		glfwPollEvents();

    }

	// Delete all the objects we've created
	defaultShader.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

    return 0; 
}