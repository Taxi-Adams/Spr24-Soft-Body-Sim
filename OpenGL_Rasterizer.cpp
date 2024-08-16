// OpenGL_Rasterizer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glad/glad.h> // obtains GPU openGL api function pointers for machine being used 
#include <GLFW/glfw3.h> // defines openGL context, handles IO and basic window operations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <vector>

#include "stb_image.h"

#include "Camera.h"

#include "VertexBuffer.h"

#include "Model.h"

#include "SimulationObject.h"

#include "ElasticCube.h"

// This simulation was initially developed using my own rasterizer, but after starting this project in the middle of some difficult development on that 
// and considering the custom reworking I would have to do for this, I implemented my simulation code into an existing example from the LearnOpenGL project.
// All simulation code is mine. This project was undertaken for a graduate computational physics course as a means to get familiar with real time simulated animation
// Collisions exhibit sensitivity to initial conditions and are sometimes prone to tangling.

// shader struct for convenient returning for ParseShader below
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1100;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 9.0f, 48.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// floor vao
unsigned int floorVAO;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    //Shader objectShader("res/shaders/vertShader.vs", "res/shaders/fragShader.fs");
    Shader objectShader("res/shaders/1.model_loading.vs", "res/shaders/1.model_loading.fs");

    Shader regShader("res/shaders/3.1.3.shadow_mapping.vs", "res/shaders/3.1.3.shadow_mapping.fs");
    Shader simpleDepthShader("res/shaders/3.1.3.shadow_mapping_depth.vs", "res/shaders/3.1.3.shadow_mapping_depth.fs");
    Shader debugDepthQuad("res/shaders/3.1.3.debug_quad.vs", "res/shaders/3.1.3.debug_quad_depth.fs");

    // commented out hardcoded values exist here after moving away from reworking the mesh data structure
    // -- the rework was becoming too large for the scope of a semester term project, so I moved to hardcoded cube meshes
    // fudging the floor from 0.0f for y coord to 0.4f for comparison testing
    float planeVertices[] = {
        // positions            // normals         // texcoords
         195.0f, 0.0f,  195.0f,  0.0f, 1.0f, 0.0f,  1.0f,  1.0f,
        -195.0f, 0.0f,  195.0f,  0.0f, 1.0f, 0.0f,   1.0f,  0.0f,
        -195.0f, 0.0f, -195.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

         195.0f, 0.0f,  195.0f,  0.0f, 1.0f, 0.0f,  1.0f,  1.0f,
        -195.0f, 0.0f, -195.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
         195.0f, 0.0f, -195.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f
    };
    // plane VAO
    unsigned int floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    int numberOfCubes = 4;
    /*ElasticCube simulCubes[4] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-3.0f, 0.0f, 3.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 3.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-2.8f, 1.0f, 6.0f, glm::vec3(0.0f, 0.0f, 0.0f))
    };*/
    /*ElasticCube simulCubes[4] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 2.0f, 0.0f)),
        ElasticCube(-3.0f, 1.0f, 4.0f, glm::vec3(3.0f, 15.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 4.3f, glm::vec3(0.0f, 14.2f, 0.0f)),
        ElasticCube(0.1f, 10.0f, -0.5f, glm::vec3(0.0f, -2.0f, 0.0f))
    };*/

    ElasticCube simulCubes[4] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.5f, 2.0f, -2.2f)),
        ElasticCube(-3.0f, 1.0f, 4.0f, glm::vec3(3.0f, 7.0f, 2.0f)),
        ElasticCube(4.8f, 1.0f, -1.1f, glm::vec3(0.0f, 14.2f, 0.0f)),
        ElasticCube(0.1f, 10.0f, -0.5f, glm::vec3(0.0f, 0.1f, 3.7f))
    };

    //int numberOfCubes = 7;
    /*ElasticCube simulCubes[7] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-3.0f, 0.0f, 3.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 3.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-2.8f, 1.0f, 6.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(14.8f, 2.0f, 5.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(1.8f, 2.0f, 8.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-9.8f, 3.0f, 2.3f, glm::vec3(0.0f, 0.0f, 0.0f))
    };*/
    /*ElasticCube simulCubes[7] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-3.0f, 0.0f, 3.0f, glm::vec3(3.0f, 15.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 3.3f, glm::vec3(0.0f, 15.0f, 0.0f)),
        ElasticCube(-2.8f, 1.0f, 4.3f, glm::vec3(4.0f, 10.0f, 1.0f)),
        ElasticCube(14.8f, 2.0f, 5.3f, glm::vec3(4.0f, 5.0f, -1.0f)),
        ElasticCube(1.8f, 2.0f, 8.3f, glm::vec3(1.0f, 2.0f, 0.4f)),
        ElasticCube(-9.8f, 3.0f, 2.3f, glm::vec3(-2.0f, 1.0f, 0.0f))
    };*/
    /*ElasticCube simulCubes[7] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 2.0f, 0.0f)),
        ElasticCube(-3.0f, 1.0f, 4.0f, glm::vec3(3.0f, 15.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 4.3f, glm::vec3(0.0f, 14.2f, 0.0f)),
        ElasticCube(0.1f, 10.0f, -0.5f, glm::vec3(0.0f, -2.0f, 0.0f)),
        ElasticCube(14.8f, 2.0f, 5.3f, glm::vec3(4.0f, 5.0f, 0.0f)),
        ElasticCube(16.8f, 2.0f, 5.5f, glm::vec3(1.0f, 5.0f, 0.0f)),
        ElasticCube(-9.8f, 3.0f, 2.3f, glm::vec3(-2.0f, 1.0f, 0.0f))
    };*/

    //int numberOfCubes = 10;
    /*ElasticCube simulCubes[10] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-3.0f, 0.0f, 3.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 3.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-2.8f, 1.0f, 6.0f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(14.8f, 2.0f, 5.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(1.8f, 2.0f, 8.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-9.8f, 3.0f, 2.3f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(14.1f, 1.0f, -3.1f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(15.8f, 2.0f, -7.5f, glm::vec3(0.0f, 0.0f, 0.0f)),
        ElasticCube(-2.8f, -1.0f, 11.8f, glm::vec3(0.0f, 0.0f, 0.0f))
    };*/
    /*ElasticCube simulCubes[10] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 0.0f)), 
        ElasticCube(-3.0f, 0.0f, 3.0f, glm::vec3(3.0f, 15.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 3.3f, glm::vec3(0.0f, 15.0f, 0.0f)), 
        ElasticCube(-2.8f, 1.0f, 4.3f, glm::vec3(4.0f, 10.0f, 1.0f)),
        ElasticCube(14.8f, 2.0f, 5.3f, glm::vec3(4.0f, 5.0f, -1.0f)),
        ElasticCube(1.8f, 2.0f, 8.3f, glm::vec3(1.0f, 2.0f, 0.4f)),
        ElasticCube(-9.8f, 3.0f, 2.3f, glm::vec3(-2.0f, 1.0f, 0.0f)), 
        ElasticCube(17.1f, 1.0f, -3.1f, glm::vec3(3.0f, 4.0f, 0.0f)),
        ElasticCube(24.8f, 2.0f, -4.3f, glm::vec3(-4.0f, 1.0f, 0.0f)),
        ElasticCube(-22.8f, -1.0f, 11.3f, glm::vec3(2.0f, 2.0f, 0.0f))
    };*/
    /*ElasticCube simulCubes[10] = {
        ElasticCube(0.0f, 0.0f, 0.0f, glm::vec3(0.0f, 2.0f, 0.0f)),
        ElasticCube(-3.0f, 1.0f, 4.0f, glm::vec3(3.0f, 15.0f, 0.0f)),
        ElasticCube(9.8f, 1.0f, 4.3f, glm::vec3(0.0f, 14.2f, 0.0f)),
        ElasticCube(0.1f, 10.0f, -0.5f, glm::vec3(0.0f, -2.0f, 0.0f)),
        ElasticCube(14.8f, 2.0f, 5.3f, glm::vec3(4.0f, 5.0f, 0.0f)),
        ElasticCube(16.8f, 2.0f, 5.5f, glm::vec3(1.0f, 5.0f, 0.0f)),
        ElasticCube(-9.8f, 3.0f, 2.3f, glm::vec3(-2.0f, 1.0f, 0.0f)),
        ElasticCube(5.1f, 1.0f, -7.1f, glm::vec3(0.0f, 1.0f, 1.0f)),
        ElasticCube(5.3f, 1.15f, -4.8f, glm::vec3(0.0f, 1.0f, -1.5f)),
        ElasticCube(-5.8f, -1.0f, 11.3f, glm::vec3(2.0f, 2.0f, 0.0f))
    };*/

    // for collision detection, we need the number of vertices and number of faces of each object being simulated, in their instantiation order
    int vertexNumberList[4] = {
        simulCubes[0].numberOfVertices,
        simulCubes[1].numberOfVertices,
        simulCubes[2].numberOfVertices,
        simulCubes[3].numberOfVertices
    };

    int faceNumberList[4] = {
        simulCubes[0].numberOfFaces,
        simulCubes[1].numberOfFaces,
        simulCubes[2].numberOfFaces,
        simulCubes[3].numberOfFaces
    };

    int numberOfObjects = numberOfCubes;

    SimulationObject simulObjects[4] = {
        simulCubes[0],
        simulCubes[1],
        simulCubes[2],
        simulCubes[3]
    };
    /*int vertexNumberList[7] = {
        simulCubes[0].numberOfVertices,
        simulCubes[1].numberOfVertices,
        simulCubes[2].numberOfVertices,
        simulCubes[3].numberOfVertices,
        simulCubes[4].numberOfVertices,
        simulCubes[5].numberOfVertices,
        simulCubes[6].numberOfVertices
    };

    int faceNumberList[7] = {
        simulCubes[0].numberOfFaces,
        simulCubes[1].numberOfFaces,
        simulCubes[2].numberOfFaces,
        simulCubes[3].numberOfFaces,
        simulCubes[4].numberOfFaces,
        simulCubes[5].numberOfFaces,
        simulCubes[6].numberOfFaces
    };

    int numberOfObjects = numberOfCubes;

    SimulationObject simulObjects[7] = {
        simulCubes[0],
        simulCubes[1],
        simulCubes[2],
        simulCubes[3],
        simulCubes[4],
        simulCubes[5],
        simulCubes[6]
    };*/
    /*int vertexNumberList[10] = {
        simulCubes[0].numberOfVertices,
        simulCubes[1].numberOfVertices,
        simulCubes[2].numberOfVertices, 
        simulCubes[3].numberOfVertices,
        simulCubes[4].numberOfVertices,
        simulCubes[5].numberOfVertices,
        simulCubes[6].numberOfVertices,
        simulCubes[7].numberOfVertices,
        simulCubes[8].numberOfVertices,
        simulCubes[9].numberOfVertices
    };

    int faceNumberList[10] = {
        simulCubes[0].numberOfFaces,
        simulCubes[1].numberOfFaces,
        simulCubes[2].numberOfFaces,
        simulCubes[3].numberOfFaces,
        simulCubes[4].numberOfFaces,
        simulCubes[5].numberOfFaces,
        simulCubes[6].numberOfFaces,
        simulCubes[7].numberOfFaces,
        simulCubes[8].numberOfFaces,
        simulCubes[9].numberOfFaces
    };

    int numberOfObjects = numberOfCubes;

    SimulationObject simulObjects[10] = {
        simulCubes[0],
        simulCubes[1],
        simulCubes[2],
        simulCubes[3],
        simulCubes[4],
        simulCubes[5],
        simulCubes[6],
        simulCubes[7],
        simulCubes[8],
        simulCubes[9]
    };*/

    // end of mesh data structure values hard coding

    // load textures
    // -------------
    char floorTextureLocation[] = "res/textures/blanket_texture.png";
    unsigned int floorTexture = loadTexture(floorTextureLocation);

    objectShader.use();

    // load models
    // -----------
    char rockLocation[] = "res/models/rock/rock.obj"; // rock exists to show off very basic rigid body implementation for visual comparison
    Model ourModel(rockLocation);

    // lighting info
    // -------------
    glm::vec3 lightPos(-1.0f, 11.0f, 0.0f);

    // start position of the test model
    glm::vec3 mdlStart = { 3.0f, 14.0f, 0.0f };
    ourModel.position = mdlStart;

    // direction of travel for the test model
    glm::vec3 mdlMove = { 4.0f, 0.0f, 0.0f }; // old x val = 0.0f
    ourModel.velocity = mdlMove;

    // ground plane normal and point for calculations
    glm::vec3 planeNormal = { 0.0f, 1.0f, 0.0f };
    glm::vec3 planePoint = { 25.0f, 0.0f, 25.0f };

    // setting object mass and radius
    ourModel.mass = 5.0f;
    ourModel.radius = 0.18f;

    glm::vec3 gravity = { 0.0f, -9.8f, 0.0f };
    ourModel.acceleration = gravity;

    float airResist = 1.5;
    float elasticCoefficient = 0.92f;
    float frictionCoeffictient = 0.15f;

    float stepSize = 0.0f;

    float simTime = 0.0f;
    float simStep = 0.0f; // step number
    float elasticSimStep = 0.0f;

    float prevDist = ourModel.position.y;

    float tolerance = 0.0001f;

    unsigned int integratorType = 0; // 0 for RK4, 1 for Symplectic Euler

    float prevTime;
    int numberFrames = 0;
    float deltaTimeSum = 0.0f;
    float totalTimeElapsed = 0.0f;

    // shadow mapping setup
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// shader configuration
    //// --------------------
    regShader.use();
    regShader.setInt("diffuseTexture", 0);
    regShader.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    // frame rate analysis tools
    prevTime = glfwGetTime();

    // render loop
    // -----------
        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            // --------------------
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            deltaTimeSum += deltaTime;
            totalTimeElapsed += deltaTime;
            if (deltaTimeSum >= 1.0f) {
                // frame rate readout
                printf("%f Seconds Elapsed\n", totalTimeElapsed);
                printf("%d -> FPS\n\n", numberFrames);
                numberFrames = 0;
                deltaTimeSum = 0.0f;
            }

            stepSize = deltaTime / 10.0f;
            simStep = 0.0f;
            simTime = lastFrame;

            // non-elastic behavior simulation loop - exists for visual comparison purposes
            while (simStep < 10.0f) {
                // basic Euler integration
                glm::vec3 oldVel = ourModel.velocity;
                glm::vec3 oldPos = ourModel.position;
                ourModel.acceleration = gravity - (airResist / ourModel.mass) * ourModel.velocity;
                ourModel.velocity += ourModel.acceleration * stepSize;
                ourModel.position += (ourModel.velocity + oldVel) * stepSize / 2.0f;

                float distance = glm::dot((ourModel.position - planePoint), planeNormal);

                // if distance has a sign change a collision has occurred.
                if (prevDist >= 0.0f && distance < 0.0f || prevDist < 0.0f && distance >= 0.0f) {
                    // determine at what timestep the collision occurred
                    float timeFraction = prevDist / (prevDist - distance);

                    // re-integrate from beginning of outer timestep at fraction of timestep
                    // integrate normally until collision, then integrate the rest of the step with new acceleration
                    ourModel.velocity = oldVel + ourModel.acceleration * stepSize * timeFraction;
                    ourModel.position = oldPos + (ourModel.velocity + oldVel) * stepSize * timeFraction / 2.0f;
                    // determining the new acceleration
                    // calculating normal and tangential velocity components
                    glm::vec3 velNormal = (glm::dot(ourModel.velocity, planeNormal)) * planeNormal;
                    glm::vec3 velTangent = ourModel.velocity - velNormal; 
                    
                    // using a simpler tangential model for quicker calculations
                    ourModel.velocity = -1 * elasticCoefficient*velNormal + (1-frictionCoeffictient)*velTangent; // no step size or += in calcs since unique velocity calc
                    ourModel.position += (ourModel.velocity + oldVel) * stepSize * (1 - timeFraction) / 2.0f; // integrate w/ new velocity for rest of partial timestep

                    // calculate updated distance
                    distance = glm::dot((ourModel.position - planePoint), planeNormal);
                }
                prevDist = distance; // update the previous distance

                // check if at rest. If so, update the position to just be the old position for now (simplest)
                if (glm::abs(ourModel.velocity.x) < 0.05f && glm::abs(ourModel.velocity.y) < 0.05f && glm::abs(ourModel.velocity.z) < 0.05f && distance < 0.05f) {
                    ourModel.velocity = glm::vec3(0.0f, 0.0f, 0.0f); // oldVel
                    ourModel.position = oldPos;
                }

                simStep += 1.0f;
            }

            // elasticity behavior included simulation loop
            elasticSimStep = 0.0f;
            while (elasticSimStep < 10.0f) {
                //testObjectCube.integrate(integratorType, stepSize);
                for (int i = 0; i < numberOfCubes; i++) {
                    simulCubes[i].integrate(integratorType, stepSize, false, false);
                }

                // check for collisions and reintegrate if necessary
                for (int i = 0; i < numberOfObjects; i++) {
                    // if a collision occurs between objects i and j, reintegrating will change position for next check, so we
                    // should use a boolean to track if a collision has occurred or not. If it has, don't simulate other collisions
                    // for the object until the next object to check against (that should work fine - its the 
                    // multiple collisions with the same object that could get hairy)
                    //bool collisionOccurred = false;
                    for (int j = 0; j < numberOfObjects; j++) {
                        // calculate a vector between both object's centers.
                        // If its length is < 2 * (object1.collisionRadius + object2.collisionRadius), check for collisions
                        // else, don't check - this is a rough performance saving measure. Also, don't check if i == j, obviously 
                        float object1to2 = glm::length(simulObjects[j].objCenter - simulObjects[i].objCenter);
                        if (object1to2 <= 0.97f * (simulObjects[j].collisionRadius + simulObjects[i].collisionRadius) && i != j) {
                            // checking object i's vertices against object j's faces
                            for (int m = 0; m < simulObjects[i].numberOfVertices; m++) {
                                for (int n = 0; n < simulObjects[j].numberOfFaces; n++) {
                                    // Calculate the time of a collision with the plane
                                    // calculate and access the: vertex's starting position pi, a vertex from the triangle, the triangle's
                                    // effective velocity, the vertex's velocity (velocities using vevlocity1), the triangle's normal (should be
                                    // the same throughout the timestep, so don't worry)
                                    glm::vec3 p_i = simulCubes[i].position1[m];
                                    glm::vec3 v_i = simulCubes[i].velocity1[m];
                                    glm::vec3 tri_p = simulCubes[j].position1[simulCubes[j].faceVertices[n][0]];

                                    glm::vec3 jFaceSide1 = simulCubes[j].position1[simulCubes[j].faceVertices[n][1]] - simulCubes[j].position1[simulCubes[j].faceVertices[n][0]];
                                    glm::vec3 jFaceSide2 = simulCubes[j].position1[simulCubes[j].faceVertices[n][2]] - simulCubes[j].position1[simulCubes[j].faceVertices[n][0]];
                                    glm::vec3 jFaceSide3 = simulCubes[j].position1[simulCubes[j].faceVertices[n][2]] - simulCubes[j].position1[simulCubes[j].faceVertices[n][1]];
                                    glm::vec3 jFaceNormal = glm::normalize(glm::cross(jFaceSide1, jFaceSide2));
                                    glm::vec3 tri_velocity = (simulCubes[j].velocity1[simulCubes[j].faceVertices[n][0]] + simulCubes[j].velocity1[simulCubes[j].faceVertices[n][1]] + simulCubes[j].velocity1[simulCubes[j].faceVertices[n][2]]) / 3.0f;

                                    glm::vec3 posDiff = tri_p - p_i;
                                    glm::vec3 normalizedPosDiff = glm::normalize(posDiff);
                                    
                                    float t_hit = glm::dot(tri_p - p_i, jFaceNormal) / glm::dot(v_i, jFaceNormal);

                                    // if t_hit is between 0 and stepSize, a hit occurred in the plane, so check if it was in the tri or not, else, do nothing
                                    // for barycentric coordinates, find area of tri, and two sub-tri's areas
                                    if (0.0f <= t_hit && t_hit <= stepSize) {
                                        glm::vec3 x_hit = p_i + v_i * t_hit;
                                        //printf("%f Plane Collision Occurred!\n", x_hit.x);
                                        float jTriArea = glm::length(glm::cross(jFaceSide1, jFaceSide2)) / 2.0f;
                                        float triArea01p = glm::length(glm::cross(x_hit - simulCubes[j].position1[simulCubes[j].faceVertices[n][0]], jFaceSide1)) / 2.0f;
                                        float triArea02p = glm::length(glm::cross(x_hit - simulCubes[j].position1[simulCubes[j].faceVertices[n][0]], jFaceSide2)) / 2.0f;
                                        float triArea12p = glm::length(glm::cross(x_hit - simulCubes[j].position1[simulCubes[j].faceVertices[n][1]], jFaceSide3)) / 2.0f;

                                        float uBary = triArea12p / jTriArea;
                                        float vBary = triArea02p / jTriArea;
                                        float wBary = triArea01p / jTriArea;

                                        // if 0 <= u,v,w <=1, x_hit is inside the triangle, and we have a hit
                                        if (0.0f <= uBary && 0.0f <= vBary && 0.0f <= wBary && uBary <= 1.0f && vBary <= 1.0f && wBary <= 1.0f) {
                                            v_i = simulCubes[i].velocity1[m];
                                            tri_velocity = (uBary * simulCubes[j].velocity1[simulCubes[j].faceVertices[n][0]] + vBary * simulCubes[j].velocity1[simulCubes[j].faceVertices[n][1]] + wBary * simulCubes[j].velocity1[simulCubes[j].faceVertices[n][2]]) / (uBary * uBary + vBary * vBary + wBary * wBary);
                                            float mass_i = simulCubes[i].particleMass;
                                            float mass_j = (uBary * simulCubes[j].particleMass + vBary * simulCubes[j].particleMass + wBary * simulCubes[j].particleMass) / (uBary * uBary + vBary * vBary + wBary * wBary);
                                            glm::vec3 centerOfMomentum = (mass_i * v_i + mass_j * tri_velocity) / (mass_i + mass_j);

                                            // calculating the velocity of each object's calculating components relative to the collision point
                                            glm::vec3 v_i_prime = v_i - centerOfMomentum;
                                            glm::vec3 v_j_prime = tri_velocity - centerOfMomentum;

                                            // calculate the velocities' normal and tangential components for before AND after the collision
                                            glm::vec3 v_i_normal_pre = glm::dot(v_i_prime, jFaceNormal) * jFaceNormal;
                                            glm::vec3 v_i_tangent_pre = v_i_prime - v_i_normal_pre;

                                            glm::vec3 v_j_normal_pre = glm::dot(v_j_prime, jFaceNormal) * jFaceNormal;
                                            glm::vec3 v_j_tangent_pre = v_j_prime - v_j_normal_pre;

                                            glm::vec3 v_i_normal_post = -1.0f * elasticCoefficient * v_i_normal_pre;
                                            glm::vec3 v_i_tangent_post = (1 - frictionCoeffictient) * v_i_tangent_pre;

                                            glm::vec3 v_j_normal_post = -1.0f * elasticCoefficient * v_j_normal_pre;
                                            glm::vec3 v_j_tangent_post = (1 - frictionCoeffictient) * v_j_tangent_pre;

                                            glm::vec3 v_i_prime_post = v_i_normal_post + v_i_tangent_post;
                                            glm::vec3 v_j_prime_post = v_j_normal_post + v_j_tangent_post;

                                            glm::vec3 v_i_post = v_i_prime_post + centerOfMomentum;
                                            glm::vec3 v_j_post = v_j_prime_post + centerOfMomentum;

                                            if (i < numberOfCubes) {
                                                simulCubes[i].velocity1[m] = v_i_post;
                                            }
                                            if (j < numberOfCubes) {
                                                glm::vec3 v_j_latter = (v_j_post - tri_velocity) / (uBary * uBary + vBary * vBary + wBary * wBary);
                                                simulCubes[j].velocity1[simulObjects[j].faceVertices[n][0]] = uBary * v_j_latter; 
                                                simulCubes[j].velocity1[simulObjects[j].faceVertices[n][1]] = vBary * v_j_latter;
                                                simulCubes[j].velocity1[simulObjects[j].faceVertices[n][2]] = wBary * v_j_latter;
                                            }
                                            // make sure to add that tolerance distance back to the integration time so we don't get off sync - should give a tiny bit more wiggle room too
                                            if (i < numberOfCubes) {
                                                simulCubes[i].integrate(integratorType, stepSize, true, false);
                                            }
                                            if (j < numberOfCubes) {
                                                simulCubes[j].integrate(integratorType, stepSize, true, false);
                                            }
                                        }
                                    }
                                }
                            }

                            // checking object i's edges against object j's edges
                            for (int m = 0; m < 12; m++) {
                                glm::vec3 edge_i_v0 = simulCubes[i].position1[simulCubes[i].strutVertexIndices[m][0]];
                                glm::vec3 edge_i_v1 = simulCubes[i].position1[simulCubes[i].strutVertexIndices[m][1]];

                                glm::vec3 edge_i = edge_i_v1 - edge_i_v0;
                                glm::vec3 edge_i_normed = glm::normalize(edge_i);
                                for (int n = 0; n < 12; n++) {
                                    glm::vec3 edge_j_v0 = simulCubes[j].position1[simulCubes[j].strutVertexIndices[n][0]];
                                    glm::vec3 edge_j_v1 = simulCubes[j].position1[simulCubes[j].strutVertexIndices[n][1]];

                                    glm::vec3 edge_j = edge_j_v1 - edge_j_v0;
                                    glm::vec3 edge_j_normed = glm::normalize(edge_j);

                                    glm::vec3 ij_normed_cross = glm::cross(edge_i, edge_j);
                                    glm::vec3 n_hat = ij_normed_cross / glm::length(ij_normed_cross);

                                    glm::vec3 i0_to_j0 = edge_j_v0 - edge_i_v0;
                                    glm::vec3 edge_i_cross_n_hat = glm::cross(glm::normalize(edge_i), n_hat);
                                    glm::vec3 edge_j_cross_n_hat = glm::cross(glm::normalize(edge_j), n_hat);

                                    float sParam = glm::dot(i0_to_j0, edge_j_cross_n_hat) / glm::dot(edge_i, edge_j_cross_n_hat);
                                    float tParam = glm::dot(-1.0f * i0_to_j0, edge_i_cross_n_hat) / glm::dot(edge_j, edge_i_cross_n_hat);

                                    // check for if there's a possibility of an actual collision (0 <= a,b <= 1)
                                    if (0.0f <= sParam && sParam <= 1.0f && 0.0f <= tParam && tParam < 1.0f) {
                                        glm::vec3 edge_i_pt = edge_i_v0 + sParam * edge_i;
                                        glm::vec3 edge_j_pt = edge_j_v0 + tParam * edge_j;

                                        float edges_iPt_to_jPt_length = glm::length(edge_j_pt - edge_i_pt);
                                        if (edges_iPt_to_jPt_length < 0.0004f) {
                                            // points are equal within a tolerance, so a collision actually happened!
                                            // calculating the barycentric constants, with u for v0, v for v1
                                            float uBary_i = glm::length(edge_i_v1 - edge_i_pt);
                                            float vBary_i = glm::length(edge_i_pt - edge_i_v0);

                                            float uBary_j = glm::length(edge_j_v1 - edge_j_pt);
                                            float vBary_j = glm::length(edge_j_pt - edge_j_v0);

                                            float edge_i_mass = (uBary_i * simulCubes[i].elasticCubeParticleMass + vBary_i * simulCubes[i].elasticCubeParticleMass) / (uBary_i * uBary_i + vBary_i * vBary_i);
                                            float edge_j_mass = (uBary_j * simulCubes[j].elasticCubeParticleMass + vBary_j * simulCubes[j].elasticCubeParticleMass) / (uBary_j * uBary_j + vBary_j * vBary_j);

                                            glm::vec3 delta_i_vel = uBary_i * simulCubes[i].velocity1[simulCubes[i].strutVertexIndices[m][0]] + vBary_i * simulCubes[i].velocity1[simulCubes[i].strutVertexIndices[m][1]] / (uBary_i * uBary_i + vBary_i * vBary_i);
                                            glm::vec3 delta_j_vel = uBary_j * simulCubes[j].velocity1[simulCubes[j].strutVertexIndices[n][0]] + vBary_j * simulCubes[j].velocity1[simulCubes[j].strutVertexIndices[n][1]] / (uBary_j * uBary_j + vBary_j * vBary_j);

                                            glm::vec3 centerOfMomentum = (edge_i_mass * delta_i_vel + edge_j_mass * delta_j_vel) / (edge_i_mass + edge_j_mass);

                                            glm::vec3 edge_i_vel_prev = delta_i_vel - centerOfMomentum;
                                            glm::vec3 edge_j_vel_prev = delta_j_vel - centerOfMomentum;

                                            glm::vec3 edge_i_vel_prev_normal = glm::dot(edge_i_vel_prev, n_hat) * n_hat;
                                            glm::vec3 edge_j_vel_prev_normal = glm::dot(edge_j_vel_prev, n_hat) * n_hat; 

                                            glm::vec3 edge_i_vel_prev_tangent = edge_i_vel_prev - edge_i_vel_prev_normal;
                                            glm::vec3 edge_j_vel_prev_tangent = edge_j_vel_prev - edge_j_vel_prev_normal;

                                            glm::vec3 edge_i_vel_post_normal = -1.0f * elasticCoefficient * edge_i_vel_prev_normal;
                                            glm::vec3 edge_j_vel_post_normal = -1.0f * elasticCoefficient * edge_j_vel_prev_normal;

                                            glm::vec3 edge_i_vel_post_tangent = (1.0f - frictionCoeffictient) * edge_i_vel_prev_tangent;
                                            glm::vec3 edge_j_vel_post_tangent = (1.0f - frictionCoeffictient) * edge_j_vel_prev_tangent;

                                            glm::vec3 edge_i_vel_after = edge_i_vel_post_normal + edge_i_vel_post_tangent;
                                            glm::vec3 edge_j_vel_after = edge_j_vel_post_normal + edge_j_vel_post_tangent;

                                            glm::vec3 edge_i_vel_post = edge_i_vel_after + centerOfMomentum;
                                            glm::vec3 edge_j_vel_post = edge_j_vel_after + centerOfMomentum;

                                            glm::vec3 edge_i_vel_latter = (edge_i_vel_post - delta_i_vel) / (uBary_i * uBary_i + vBary_i * vBary_i);
                                            glm::vec3 edge_j_vel_latter = (edge_j_vel_post - delta_j_vel) / (uBary_j * uBary_j + vBary_j * vBary_j);

                                            // reintegrate
                                            simulCubes[i].velocity1[simulCubes[i].strutVertexIndices[m][0]] = uBary_i * edge_i_vel_latter;
                                            simulCubes[i].velocity1[simulCubes[i].strutVertexIndices[m][1]] = vBary_i * edge_i_vel_latter;

                                            simulCubes[j].velocity1[simulCubes[j].strutVertexIndices[n][0]] = uBary_j * edge_j_vel_latter;
                                            simulCubes[j].velocity1[simulCubes[j].strutVertexIndices[n][1]] = vBary_j * edge_j_vel_latter;

                                            simulCubes[i].integrate(integratorType, stepSize, true, false);
                                            simulCubes[j].integrate(integratorType, stepSize, true, false);
                                        }
                                    }
                                }
                            }
                        } 
                    }

                    if (!simulCubes[i].atRest && simulCubes[i].position1[0].y < 0.04f || simulCubes[i].position1[1].y < 0.04f || simulCubes[i].position1[2].y < 0.04f || simulCubes[i].position1[3].y < 0.04f || simulCubes[i].position1[4].y < 0.04f || simulCubes[i].position1[5].y < 0.04f || simulCubes[i].position1[6].y < 0.04f || simulCubes[i].position1[7].y < 0.04f) {
                        for (int j = 0; j < simulObjects[i].numberOfVertices; j++) {
                            float groundDistanceBefore = glm::dot((simulCubes[i].position1[j] - planePoint), planeNormal);
                            float groundDistanceAfter = glm::dot((simulCubes[i].integratedPosition[j] - planePoint), planeNormal);

                            if (groundDistanceBefore >= -0.006f && groundDistanceAfter < 0.0f) {
                                float timeFraction = groundDistanceBefore / (groundDistanceBefore - groundDistanceAfter);

                                glm::vec3 v_i_normal_pre = glm::dot(simulCubes[i].velocity1[j], planeNormal) * planeNormal;
                                glm::vec3 v_i_tangent_pre = simulCubes[i].velocity1[j] - v_i_normal_pre;

                                glm::vec3 v_i_normal_post = -1.0f * elasticCoefficient * v_i_normal_pre;
                                glm::vec3 v_i_tangent_post = (1 - frictionCoeffictient) * v_i_tangent_pre;

                                glm::vec3 v_i_post = v_i_normal_post + v_i_tangent_post;
                                // go ahead and apply a small jolt to any verts below 0.07 along their y - cheating, but given the essential collision detection only being done, it can help
                                for (int m = 0; m < 8; m++) {
                                    simulCubes[i].velocity1[m].y += v_i_post.y;
                                }
                                simulCubes[i].velocity1[j] = v_i_post; // go ahead and ensure that the simulated vertex gets its proper velocity
                                simulCubes[i].integrate(integratorType, stepSize, true, true);
                            }
                        }
                    }
                }
                elasticSimStep += 1.0f;
            }


            // input
            processInput(window);

            // render
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // shadow map code from Learn OpenGL project - my other rasterizer branch is in the middle of reworking this
            // 1. render depth of scene to texture (from light's perspective)
            // --------------------------------------------------------------
            glm::mat4 lightProjection, lightView;
            glm::mat4 lightSpaceMatrix;
            float near_plane = 0.5f, far_plane = 17.5f; // orig near = 1.0f, far = 7.5f
            //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
            lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
            lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            lightSpaceMatrix = lightProjection * lightView;
            // render scene from light's point of view
            simpleDepthShader.use();
            simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, ourModel.position);
            model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
            //objectShader.setMat4("model", model);
            simpleDepthShader.setMat4("model", model);
            ourModel.Draw(simpleDepthShader);

            //glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            glm::mat4 floorModel = glm::mat4(1.0f);
            floorModel = glm::translate(floorModel, glm::vec3(0.0f, 0.0f, 0.0f));
            //objectShader.setMat4("model", floorModel);
            simpleDepthShader.setMat4("model", floorModel);
            glBindVertexArray(floorVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            //glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // hardcoded values crop up here too. TO FIX if reworked outside of school
            //glm::mat4 cubeiModel[10];
            //glm::mat4 cubeiModel[7];
            glm::mat4 cubeiModel[4];
            for (int i = 0; i < numberOfCubes; i++) {
                glBindTexture(GL_TEXTURE_2D, floorTexture);
                cubeiModel[i] = glm::mat4(1.0f);
                simpleDepthShader.setMat4("model", cubeiModel[i]);
                glBindVertexArray(simulCubes[i].cubeVAO);

                glBindBuffer(GL_ARRAY_BUFFER, simulCubes[i].cubeVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(simulCubes[i].elasticCubeVertices), simulCubes[i].elasticCubeVertices, GL_DYNAMIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, simulCubes[i].cubeEBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(simulCubes[i].elasticCubeIndices), simulCubes[i].elasticCubeIndices, GL_DYNAMIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            // reset viewport
            glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // don't forget to enable shader before setting uniforms
            regShader.use();

            // view/projection transformations
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            regShader.setMat4("projection", projection);
            regShader.setMat4("view", view);

            // set light uniforms
            regShader.setVec3("viewPos", camera.Position);
            regShader.setVec3("lightPos", lightPos);
            regShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floorTexture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);


            // render the loaded model
            model = glm::mat4(1.0f);
            model = glm::translate(model, ourModel.position);
            model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
            regShader.setMat4("model", model);
            ourModel.Draw(objectShader);

            glBindTexture(GL_TEXTURE_2D, floorTexture);
            floorModel = glm::mat4(1.0f);
            floorModel = glm::translate(floorModel, glm::vec3(0.0f, 0.0f, 0.0f));
            regShader.setMat4("model", floorModel);
            glBindVertexArray(floorVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            for (int i = 0; i < numberOfCubes; i++) {
                glBindTexture(GL_TEXTURE_2D, floorTexture);
                cubeiModel[i] = glm::mat4(1.0f);
                regShader.setMat4("model", cubeiModel[i]);
                glBindVertexArray(simulCubes[i].cubeVAO);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }

            lastFrame = currentFrame;

            // render Depth map to quad for visual debugging
            //debugDepthQuad.use();
            //debugDepthQuad.setFloat("near_plane", near_plane);
            //debugDepthQuad.setFloat("far_plane", far_plane);
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, depthMap);
            numberFrames += 1;

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
        return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
