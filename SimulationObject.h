#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>
using namespace std;
class SimulationObject
{
public:
    int numberOfVertices;
    int numberOfFaces; 
    int objectType; // 0 = cube, 1 = pyramid, 2 = diamond
    glm::vec3 objCenter;
    float collisionRadius;
    // either use all of them or not depending on shape, numberOfFaces variable allows simulatio to only iter through needed
    unsigned int faceVertices[12][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    glm::vec3 position1[8];
    glm::vec3 velocity1[8];
    glm::vec3 integratedPosition[8];
    float particleMass;
    bool atRest;
};

