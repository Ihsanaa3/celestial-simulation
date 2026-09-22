#include"mesh.h" 
#include"extraFunctions.h" 
#include"OrbitTrail.h" 
#include"meshDrawTools.h"


const unsigned int width = 1920;
const unsigned int height = 1080; 

const glm::mat4 iden4 = glm::mat4(1.0f); 

const float sunRadius = 1.9f; 
const float earthRadius = 1.0f; 
const float moonRadius = 0.7f; 

glm::vec3 sunColour   = glm::vec3(1.00f, 0.84f, 0.00f); // Bright Sun Gold (#FFD700)
glm::vec3 earthColour = glm::vec3(0.12f, 0.56f, 1.00f); // Ocean Blue (#1E90FF)
glm::vec3 moonColour  = glm::vec3(0.75f, 0.75f, 0.75f); // Lunar Gray (#BFBFBF) 

int main(){
    GLFWwindow* window = et::initialiseGLFW(width, height, "Window" ); 
    glfwSwapInterval(1); // VSYNC 1 = off, 0 = on

    Shader lightShader("C:/OGL/res/vertex/light.vert.txt", "C:/OGL/res/fragment/light.frag.txt");

    Mesh sun = drt::createSphere(sunRadius, 50, 50, sunColour); 

    Shader shaderProgram("C:/OGL/res/vertex/mat.vert.txt", "C:/OGL/res/fragment/litcolour.frag.txt"); 

    Mesh earth = drt::createSphere(earthRadius, 20, 20, earthColour); 
    Mesh moon = drt::createSphere(moonRadius, 10, 10, moonColour);  

    Shader defaultShader("C:/OGL/res/vertex/udef.vert.txt", "C:/OGL/res/fragment/default.frag.txt"); 
    
    Mesh grid = drt::createAxis(glm::vec3(0.0, 0.5, 1.0), glm::vec3(0.0, 1.0, 0.5), 25); 

    Shader trailShader("C:/OGL/res/vertex/udef.vert.txt", "C:/OGL/res/fragment/trail.frag.txt"); 

    OrbitTrail earthTrail(7300, earthColour); 
    OrbitTrail moonTrail(2000,  moonColour); 
    
    glEnable(GL_DEPTH_TEST); 
    
    Camera camera(width, height, glm::vec3(0.0f, 10.0f, 75.0f));

    double prevTime = glfwGetTime(); 
    double previousTime = prevTime;  
    int frameRate = 0; 
    float angle = 0.0f; 

    glm::vec3 earthPos = glm::vec3(0.0f, 0.0f, 0.0f); 
    glm::vec3 moonPos = glm::vec3(0.0f, 0.0f, 0.0f); 

    while (!glfwWindowShouldClose(window))
	{
		// Specify the  of the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double crntTime = glfwGetTime(); 
        frameRate += 1; 

        if (crntTime - prevTime >= 1.0f){
            std::cout << "Frame Rate: " << frameRate << std::endl;  
            std::cout << "  " << std::endl; 

            prevTime = crntTime; 
            frameRate = 0;  

        }if (crntTime - previousTime >= 1.0f / 120.0f){
            // std::cout << "earthPos: " << earthPos.x << " " << earthPos.y << " " << earthPos.z << std::endl; 
            // std::cout << "moonPos: " << moonPos.x << " " << moonPos.y << " " << moonPos.z << std::endl; 
            angle += 2 * PI / (365 * 20); 
            previousTime = crntTime; 
        } 

        float earthSelfAngle  = angle * 20.0f;  // Earth spinning fast on axis
        float earthOrbitAngle = angle;  // Earth orbiting slow around Sun
        float moonSelfAngle   = angle * 1.5f;  // Moon spinning on axis
        float moonOrbitAngle  = angle * (365.0f / 29.5f);  // Moon orbiting fast around Earth

   		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 200.0f); 

        camera.Inputs(window); 

        // Earth Matrix = Earth Orbit Roation X Earth Position Offset X Earth Self Rotation 
        // Moon Matrix = Earth Orbit Roation X Earth Position Offset X Moon Orbit Rotation X Moon Ofset X Moon Self Rotation 
       
        // Matrices for Sun 
		glm::vec4 lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f );
		glm::mat4 lightModel = iden4;
		lightModel = glm::translate(lightModel, lightPos);

        lightShader.Activate();
		glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w); 
        sun.Draw(lightShader, camera, GL_TRIANGLES); // DRAW SUN 

        // Earth Matrices 
        glm::mat4 earthOrbit = glm::rotate(iden4, earthOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
        glm::mat4 earthTranslate = glm::translate(iden4, glm::vec3(15.0f, 0.0f, 5.0f)); 
        glm::mat4 earthSelfRot = glm::rotate(iden4, earthSelfAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 

        // Combined Earth Model Matrix
        glm::mat4 earthModel = earthOrbit * earthTranslate * earthSelfRot;

        // Calculate Normal Matrix for Earth
        glm::mat3 earthNormalMatrix = glm::transpose(glm::inverse(glm::mat3(earthModel)));

        // Calculate position vector of earth to add to the trail 
        earthPos = glm::vec3(earthModel[3]); 
        earthTrail.updateTrail(earthPos); 

		shaderProgram.Activate();
		// Calculate the normal matrix from objectModel
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(earthModel)));
		// Send uniforms to shaderProgram
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(earthModel));
		glUniformMatrix3fv(glGetUniformLocation(shaderProgram.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(earthNormalMatrix)); 
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z); 

		earth.Draw(shaderProgram, camera, GL_TRIANGLES); // DRAW EARTH 

        // Moon Matrices   
        glm::mat4 moonOrbit = glm::rotate(iden4, moonOrbitAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
        glm::mat4 moonTranslate = glm::translate(iden4, glm::vec3(3.0f, 0.0f, 5.0f)); 
        glm::mat4 moonSelfRot = glm::rotate(iden4, moonSelfAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 

        // Earth's world position transformation (where the Earth is in space)
        glm::mat4 earthWorld = earthOrbit * earthTranslate; 

        // Moon's local transformation relative to Earth
        glm::mat4 moonLocal = moonOrbit * moonTranslate * moonSelfRot;

        // Combined them
        glm::mat4 moonModel = earthWorld * moonLocal;

        // Calculate Normal Matrix for Earth
        glm::mat3 moonNormalMatrix = glm::transpose(glm::inverse(glm::mat3(moonModel)));

        // Calculate position vector of moon to add to the trail 
        moonPos = glm::vec3(moonModel[3]);  
        moonTrail.updateTrail(moonPos);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(moonModel));
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(moonNormalMatrix));

        // Draw Moon now so it uses Moon's model matrix
        moon.Draw(shaderProgram, camera, GL_TRIANGLES);

		glm::mat4 gridObjModel = glm::rotate(iden4, angle * 10.0f, glm::vec3(0.0f, 1.0f, 0.0f)); 

        defaultShader.Activate(); 

        glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(gridObjModel));

        grid.Draw(defaultShader, camera, GL_LINES); // DRAW AXIS 

        earthTrail.drawTrail(trailShader, camera); 
        moonTrail.drawTrail(trailShader, camera); 

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
        // lets you esc out 
		et::processInput(window); 
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
    lightShader.Delete();  
    defaultShader.Delete(); 
    
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	return 0;
}