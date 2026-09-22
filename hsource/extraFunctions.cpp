#include "extraFunctions.h"

// sets a seed
void rd::randseed() { 
    std::srand(static_cast<unsigned int>(std::time(nullptr))); 
}

// returns +1 or -1 
int rd::ransign(){
    int x = rand() % 2; 
    if (x == 0){
        return -1;
    } 
    return 1; 
}

// returns a random integer
int rd::randIntSign(int ub){
    int x = rand() % (ub+1); 
    return (x * ransign()); 
}

// returns a random float 
float rd::randsignf(){ 
    float x = static_cast <float> (rand())/static_cast <float> (RAND_MAX) ; 
    return (x * ransign()); 
}

// returns a random positive float
float rd::randf(){
    float x = static_cast<float>(rand())/ static_cast <float> (RAND_MAX); 
    return x; 
}

// returns a random positive float less than the upper bound
float rd::randfb(float upper_bound){
    float x = static_cast <float> (rand())/static_cast <float> (RAND_MAX/upper_bound) ; 
    return (x * ransign()); 
}

void et::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height); 
} // allows windows to be resized 

void et::processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true); 
    }
} // terminates window if esc pressed

void et::versioncheck(){
    const GLubyte* vendor   = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);

    std::cout << "GPU Vendor: " << vendor << std::endl;
    std::cout << "Device: " << renderer << std::endl;
    std::cout << "OpenGL Version: " << version << std::endl;
} // prints hardware and version in use  

GLFWwindow* et::initialiseGLFW(int width, int height, const char* title) {
    
    // 1. Initialize GLFW library
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    // 2. Set OpenGL context versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3. Create window handle
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);

    // 4. Load OpenGL functions via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    // 5. Configure window render settings & callbacks
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, et::framebuffer_size_callback);
    glfwSwapInterval(1); // Enable VSync

    return window;
}