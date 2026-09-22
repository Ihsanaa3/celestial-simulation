#include"mesh.h" 
#include"extraFunctions.h" 
#include"OrbitTrail.h" 
#include"meshDrawTools.h" 
#include"relativeResPath.h" 

const unsigned int width = 900;
const unsigned int height = 900; 

const glm::mat4 iden4 = glm::mat4(1.0f); 
const float G = 1e1; 
const float radius = 0.05f; 
const int resolution = 18; 
const int noObjects = 6; 

class Ball {
public:
    Mesh circleMesh; 
    OrbitTrail ballTrail; 
    bool stationary = false; 
    float mass = 1.0f;
    glm::vec2 position = glm::vec2(0.0f); 
    glm::vec2 velocity = glm::vec2(0.0f); 

    // Constructor runs AFTER OpenGL is initialized in main()
    Ball() : circleMesh(drt::createCircle(radius, resolution)), 
             ballTrail(25)
    {} 
    
};


int main(){
    GLFWwindow* window = et::initialiseGLFW(width, height, "Window"); 
    rd::randseed(); // sets the seed 
    // glfwSwapInterval(0); 

    Shader trailShader(rrp::getVertexPath() + "modelmat.vert.txt", rrp::getFragmentPath() + "trail.frag.txt"); 

    Shader ballShader(rrp::getVertexPath() + "modelmat.vert.txt", rrp::getFragmentPath() + "default.frag.txt"); 
    
    std::vector<Ball> AllOfMyBalls;  
    AllOfMyBalls.reserve(noObjects); 

    float randradius = 0.1f;// * rd::randf(); 
    
    for (int i = 0; i < noObjects; ++i) {
        AllOfMyBalls.emplace_back();          // Create the ball in the vector
        
        // float ranPosX = rd::randsignf();
        // float ranPosY = rd::randsignf();
        // AllOfMyBalls.back().position = glm::vec2(ranPosX, ranPosY);  
        glm::vec3 colour = glm::vec3(rd::randf(), rd::randf(), rd::randf()); 
        AllOfMyBalls.back().circleMesh = drt::createCircle(radius, resolution, colour);
        AllOfMyBalls.back().ballTrail.setColor(colour); 
        float nx, ny; 
        float angle; 
    
        angle = i * PI; //* 2 * PI / (noObjects); 
        nx = randradius * cosf(angle); 
        ny = -randradius * sin(angle); 
    
        AllOfMyBalls.back().position = glm::vec2(nx, ny); 
        if(i % 2 == 0){
            AllOfMyBalls.back().velocity = glm::vec2(-0.01f, 0.0f); 
            randradius += 0.1;
        }
        else{
            AllOfMyBalls.back().velocity = glm::vec2(0.01f, 0.0f);
        } 

    }   

    // AllOfMyBalls.back().circleMesh = drt::createCircle(0.15f, 25); 
    // AllOfMyBalls.back().position = glm::vec2(0.0f, 0.0f); 
    // AllOfMyBalls.back().mass = 2.0f; 
    // AllOfMyBalls.back().stationary = true; 

    double prevTime = glfwGetTime(); 
    double previousTime = prevTime;  
    int frameRate = 0; 
    float diameter = 2 * radius; 
    glm::vec2 acceleration = glm::vec2(0.0f, 0.0f); 
    float dt = 1.0f / 60.0f; 

    while (!glfwWindowShouldClose(window)){
        // Specify the  of the background
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer 
		glClear(GL_COLOR_BUFFER_BIT);

        double crntTime = glfwGetTime(); 
        frameRate += 1; 

        if (crntTime - prevTime >= 1.0f){
            std::cout << "Frame Rate: " << frameRate << std::endl;  
            prevTime = crntTime; 
            frameRate = 0;  
        }
        if (crntTime - previousTime >= 1.0f / 60.0f){
            previousTime = crntTime; 
            for(Ball& ball: AllOfMyBalls){ 

                for(Ball& ball2: AllOfMyBalls){
                    float dist = glm::distance(ball.position, ball2.position); 
                    glm::vec2 midpoint = glm::vec2((ball.position.x + ball2.position.x)/2, (ball.position.y + ball2.position.y) / 2); 
                    if(dist < diameter){

                        std::cout << "collision" << std::endl;
                        ball.position = midpoint + glm::vec2(radius,0);
                        ball2.position = midpoint - glm::vec2(radius,0);                            
                        ball.velocity.x *= -1; 
                        ball2.velocity.x *= -1; 
                    }
                }

                ball.position += ball.velocity; 
                float minusrd = -1.0f + radius;
                float positiverd = 1.0f - radius;
                // Check X boundaries
                if (ball.position.x < minusrd) {
                    ball.position.x = -1.0f + radius;          // Clamp to boundary to prevent sticking
                    ball.velocity.x *= -1.0f;         // Reverse X velocity
                } 
                else if (ball.position.x > positiverd) {
                    ball.position.x = 1.0f - radius;           // Clamp to boundary to prevent sticking
                    ball.velocity.x *= -1.0f;         // Reverse X velocity
                }

                // Check Y boundaries
                if (ball.position.y < minusrd) {
                    ball.position.y = -1.0f + radius;          // Clamp to boundary to prevent sticking
                    ball.velocity.y *= -1.0f;         // Reverse Y velocity
                } 
                else if (ball.position.y > positiverd) {
                    ball.position.y = 1.0f - radius;           // Clamp to boundary to prevent sticking
                    ball.velocity.y *= -1.0f;         // Reverse Y velocity
                }

            }
        } 

        
        for(Ball& ball: AllOfMyBalls){ 
            
            ballShader.Activate(); 

            // glm::mat4 model = iden4; 
            glm::mat4 ballStartTranslate = glm::translate(iden4, glm::vec3(ball.position, 0.0f)); 
            // GLint modelLoc = glGetUniformLocation(ballShader.ID, "model"); 
            // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ballStartTranslate));
            glUniformMatrix4fv(glGetUniformLocation(ballShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(ballStartTranslate));
            ball.circleMesh.Draw(ballShader, GL_TRIANGLES); 
            
            trailShader.Activate(); 
            ball.ballTrail.drawTrail(trailShader); 
        }
        
        // Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
        // lets you esc out 
		et::processInput(window); 
		// Take care of all GLFW events
		glfwPollEvents();

    }
    ballShader.Delete(); 
    
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();


    return 0; 
    
}