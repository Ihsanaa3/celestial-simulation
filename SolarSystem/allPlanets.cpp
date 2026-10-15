#include "mesh.h" 
#include "extraFunctions.h" 
#include "OrbitTrail.h" 
#include "meshDrawTools.h"
#include "relativeResPath.h"

class Planet {
public:
    std::string name;
    Mesh mesh;
    Mesh ringMesh;        
    bool hasRings;
    OrbitTrail trail;
    float radius;
    float distance;        
    float orbitSpeed;      
    float selfRotSpeed;    
    glm::vec3 color;
    
    std::vector<Planet> moons;

    Planet(std::string name, float radius, float distance, float orbitSpeed, float selfRotSpeed, glm::vec3 color, int trailCapacity, bool hasRings = false, float ringRadius = 0.0f)
        : name(name), radius(radius), distance(distance), orbitSpeed(orbitSpeed), selfRotSpeed(selfRotSpeed), color(color),
          hasRings(hasRings),
          mesh(drt::createSphere(radius, 25, 25, color)),
          ringMesh(hasRings ? drt::createSphere(ringRadius, 30, 30, color * 0.8f) : drt::createSphere(0.1f, 4, 4, color)),
          trail(trailCapacity, color) {}

    void updateAndDraw(float angle, const glm::mat4& parentMatrix, Shader& shader, Camera& camera, glm::vec4 lightCol, glm::vec3 lightPos) {
        float currentOrbitAngle = angle * orbitSpeed;
        float currentSelfAngle = angle * selfRotSpeed;

        // Changed rotation axis to Z-axis (0.0f, 0.0f, 1.0f) for XY plane alignment
        glm::mat4 orbitMat = glm::rotate(glm::mat4(1.0f), currentOrbitAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(distance, 0.0f, 0.0f));
        glm::mat4 selfRotMat = glm::rotate(glm::mat4(1.0f), currentSelfAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 modelMatrix = parentMatrix * orbitMat * translateMat * selfRotMat;

        glm::vec3 worldPos = glm::vec3(modelMatrix[3]);
        trail.updateTrail(worldPos);

        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

        // Draw Planet Body
        shader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix3fv(glGetUniformLocation(shader.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
        glUniform4f(glGetUniformLocation(shader.ID, "lightColour"), lightCol.x, lightCol.y, lightCol.z, lightCol.w);
        glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
        mesh.Draw(shader, camera, GL_TRIANGLES);

        // Draw Saturn's Rings if enabled
        if (hasRings) {
            glm::mat4 ringModel = modelMatrix * glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f))
                                             * glm::scale(glm::mat4(1.0f), glm::vec3(1.8f, 1.8f, 0.05f));
            glm::mat3 ringNormal = glm::transpose(glm::inverse(glm::mat3(ringModel)));
            
            glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(ringModel));
            glUniformMatrix3fv(glGetUniformLocation(shader.ID, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(ringNormal));
            ringMesh.Draw(shader, camera, GL_TRIANGLES);
        }

        for (auto& moon : moons) {
            glm::mat4 planetWorldBase = parentMatrix * orbitMat * translateMat;
            moon.updateAndDraw(angle, planetWorldBase, shader, camera, lightCol, lightPos);
        }
    }

    void drawTrails(Shader& trailShader, Camera& camera) {
        trail.drawTrail(trailShader, camera);
        for (auto& moon : moons) {
            moon.drawTrails(trailShader, camera);
        }
    }
};

const unsigned int width = 1920;
const unsigned int height = 1080; 

int main(){
    GLFWwindow* window = et::initialiseGLFW(width, height, "Solar System Simulation"); 
    glfwSwapInterval(1); 

    Shader lightShader(rrp::getVertexPath() + "light.vert.txt", rrp::getFragmentPath() + "light.frag.txt");
    Shader shaderProgram(rrp::getVertexPath() + "mat.vert.txt", rrp::getFragmentPath() + "litcolour.frag.txt"); 
    Shader defaultShader(rrp::getVertexPath() + "udef.vert.txt", rrp::getFragmentPath() + "default.frag.txt"); 
    Shader trailShader(rrp::getVertexPath() + "udef.vert.txt", rrp::getFragmentPath() + "trail.frag.txt"); 

    const float sunRadius = 3.5f; 
    glm::vec3 sunColour = glm::vec3(1.00f, 0.84f, 0.00f); 
    Mesh sun = drt::createSphere(sunRadius, 50, 50, sunColour); 

    Mesh grid = drt::createAxis(100.0f, glm::vec3(0.0, 0.5, 1.0), glm::vec3(0.0, 1.0, 0.5)); 
    glEnable(GL_DEPTH_TEST); 
    
    // Moved camera directly in front along the Z-axis facing the XY plane
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 120.0f));

    std::vector<Planet> planets;

    planets.emplace_back("Mercury", 0.4f, 8.0f, 4.15f, 10.0f, glm::vec3(0.7f, 0.7f, 0.7f), 1000);
    planets.emplace_back("Venus", 0.8f, 13.0f, 1.62f, 5.0f, glm::vec3(0.9f, 0.7f, 0.4f), 1500);

    Planet earth("Earth", 1.0f, 19.0f, 1.00f, 20.0f, glm::vec3(0.12f, 0.56f, 1.00f), 3000);
    earth.moons.emplace_back("Moon", 0.3f, 2.5f, 12.3f, 2.0f, glm::vec3(0.75f, 0.75f, 0.75f), 500);
    planets.push_back(earth);

    Planet mars("Mars", 0.6f, 26.0f, 0.53f, 18.0f, glm::vec3(0.80f, 0.30f, 0.10f), 4000);
    mars.moons.emplace_back("Phobos", 0.15f, 1.2f, 20.0f, 5.0f, glm::vec3(0.5f, 0.5f, 0.5f), 200);
    mars.moons.emplace_back("Deimos", 0.10f, 1.8f, 15.0f, 4.0f, glm::vec3(0.6f, 0.6f, 0.6f), 200);
    planets.push_back(mars);

    Planet jupiter("Jupiter", 2.2f, 38.0f, 0.08f, 45.0f, glm::vec3(0.85f, 0.65f, 0.45f), 6000);
    jupiter.moons.emplace_back("Io", 0.25f, 3.2f, 14.0f, 3.0f, glm::vec3(0.9f, 0.8f, 0.3f), 300);
    jupiter.moons.emplace_back("Europa", 0.22f, 4.2f, 11.0f, 3.0f, glm::vec3(0.8f, 0.9f, 0.9f), 300);
    jupiter.moons.emplace_back("Ganymede", 0.35f, 5.5f, 8.0f, 2.5f, glm::vec3(0.6f, 0.6f, 0.6f), 300);
    jupiter.moons.emplace_back("Callisto", 0.30f, 6.8f, 6.0f, 2.0f, glm::vec3(0.4f, 0.4f, 0.4f), 300);
    jupiter.moons.emplace_back("Amalthea", 0.12f, 2.4f, 18.0f, 4.0f, glm::vec3(0.5f, 0.4f, 0.3f), 150);
    planets.push_back(jupiter);

    Planet saturn("Saturn", 1.8f, 52.0f, 0.03f, 40.0f, glm::vec3(0.90f, 0.80f, 0.60f), 7000, true, 2.4f);
    saturn.moons.emplace_back("Titan", 0.32f, 4.5f, 9.0f, 2.0f, glm::vec3(0.8f, 0.7f, 0.4f), 300);
    planets.push_back(saturn);

    planets.emplace_back("Uranus", 1.2f, 65.0f, 0.01f, 25.0f, glm::vec3(0.40f, 0.75f, 0.85f), 8000);
    planets.emplace_back("Neptune", 1.2f, 76.0f, 0.006f, 28.0f, glm::vec3(0.20f, 0.40f, 0.80f), 9000);

    double prevTime = glfwGetTime(); 
    int frameRate = 0; 
    float angle = 0.0f; 

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        double crntTime = glfwGetTime(); 
        frameRate += 1; 

        if (crntTime - prevTime >= 1.0) {
            std::cout << "Frame Rate: " << frameRate << std::endl; 
            prevTime = crntTime; 
            frameRate = 0; 
        }

        angle += 0.001f; // Slowed down a bit so it's easier to watch head-on!

        camera.updateMatrix(45.0f, 0.1f, 300.0f); 
        camera.Inputs(window); 

        glm::vec4 lightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::mat4 lightModel = glm::mat4(1.0f);

        lightShader.Activate();
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniform4f(glGetUniformLocation(lightShader.ID, "lightColour"), lightColour.x, lightColour.y, lightColour.z, lightColour.w); 
        sun.Draw(lightShader, camera, GL_TRIANGLES); 

        glm::mat4 rootMatrix = glm::mat4(1.0f);
        for (auto& planet : planets) {
            planet.updateAndDraw(angle, rootMatrix, shaderProgram, camera, lightColour, lightPos);
        }

        glm::mat4 gridObjModel = glm::rotate(glm::mat4(1.0f), angle * 0.1f, glm::vec3(0.0f, 0.0f, 1.0f)); 
        defaultShader.Activate(); 
        glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(gridObjModel));
        grid.Draw(defaultShader, camera, GL_LINES); 

        for (auto& planet : planets) {
            planet.drawTrails(trailShader, camera);
        }

        glfwSwapBuffers(window);
        et::processInput(window); 
        glfwPollEvents();
    }

    shaderProgram.Delete();
    lightShader.Delete();  
    defaultShader.Delete(); 
    trailShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}