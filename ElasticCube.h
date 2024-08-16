#include "SimulationObject.h"
class ElasticCube: public SimulationObject
{
public:
    // more hard coded data for the sake of a quicker implementation
    float elasticCubeMass = 2.0f;
    float elasticCubeParticleMass = 0.25f;
    float elasticCubeDragCoeff = 0.1f;
    float elasticCubeLiftCoeff = 0.02f;
    float springPeriod = 3.0f;
    float springTimeConstant = 4.0f;
    float elasticCubeElasticCoeff = 4.0f * elasticCubeParticleMass * pow(3.1415f, 2) / pow(springPeriod, 2);
    float elasticCubeDampingCoeff = 2.0f * elasticCubeParticleMass / springTimeConstant;
    //int numberOfFaces; // 2 tri's per cube face - functionally 24 total due to simulation needs

    unsigned int cubeVAO, cubeVBO, cubeEBO;

    unsigned int strutVertexIndices[28][2] = {
        // edge 0
        {4,5},
        // edge 1
        {5,7},
        // edge 2
        {6,7},
        // edge 3
        {4,6},
        // edge 4
        {0,1},
        // edge 5
        {1,3},
        // edge 6
        {2,3},
        // edge 7
        {0,2},
        // edge 8
        {0,4},
        // edge 9
        {1,5},
        // edge 10
        {2,6},
        // edge 11
        {3,7},
        // edge 12 diagonal front, left, back, right, top, bottom
        {4,7},
        // edge 13
        {3,5},
        // edge 14
        {0,3},
        // edge 15
        {0,6},
        // edge 16
        {2,7},
        // edge 17
        {0,5},
        // edge 18
        {5,6},
        // edge 19
        {1,7},
        // edge 20
        {1,2},
        // edge 21
        {2,4},
        // edge 22
        {3,6},
        // edge 23
        {1,4},
        // edge 24 (inside)
        {0,7},
        // edge 25 (inside)
        {1,6},
        // edge 26 (inside)
        {3,4},
        // edge 27 (inside)
        {2,5}
    };

    unsigned int strutFaceIndices[24][2] = {
        // edge 0
        {0,10},
        // edge 1
        {0,7},
        // edge 2
        {1,8},
        // edge 3
        {1,4},
        // edge 4
        {2,11},
        // edge 5
        {2,6},
        // edge 6
        {3,9},
        // edge 7
        {3,5},
        // edge 8
        {4,10},
        // edge 9
        {6,11},
        // edge 10
        {5,8},
        // edge 11
        {7,9},
        // edge 12
        {0,1},
        // edge 13
        {6,7},
        // edge 14
        {2,3},
        // edge 15
        {4,5},
        // edge 16
        {8,9},
        // edge 17
        {10,11},
        // edge 18
        {12,13},
        // edge 19
        {18,19},
        // edge 20
        {14,15},
        // edge 21
        {16,17},
        // edge 22
        {20,21},
        // edge 23
        {22,23}
    };

    float strutLengths[28] = {
        // edge 0
        2.0f,
        // edge 1
        2.0f,
        // edge 2
        2.0f,
        // edge 3
        2.0f,
        // edge 4
        2.0f,
        // edge 5
        2.0f,
        // edge 6
        2.0f,
        // edge 7
        2.0f,
        // edge 8
        2.0f,
        // edge 9
        2.0f,
        // edge 10
        2.0f,
        // edge 11
        2.0f,
        // edge 12
        2.828f,
        // edge 13
        2.828f,
        // edge 14
        2.828f,
        // edge 15
        2.828f,
        // edge 16
        2.828f,
        // edge 17
        2.828f,
        // edge 18
        2.828f,
        // edge 19
        2.828f,
        // edge 20
        2.828f,
        // edge 21
        2.828f,
        // edge 22
        2.828f,
        // edge 23
        2.828f,
        // edge 24
        3.464f,
        // edge 25
        3.464f,
        // edge 26
        3.464f,
        // edge 27
        3.464f
    };

    // scale by 1.414 for non diagonals!
    // k = 0.2f, nondiagonal edges (0 - 11) scaled by 2.828/2, elasticity - should be lower
    float strutStiffness[28] = {
        // edge 0
        elasticCubeElasticCoeff * 1.732f,
        // edge 1
        elasticCubeElasticCoeff * 1.732f,
        // edge 2
        elasticCubeElasticCoeff * 1.732f,
        // edge 3
        elasticCubeElasticCoeff * 1.732f,
        // edge 4
        elasticCubeElasticCoeff * 1.732f,
        // edge 5
        elasticCubeElasticCoeff * 1.732f,
        // edge 6
        elasticCubeElasticCoeff * 1.732f,
        // edge 7
        elasticCubeElasticCoeff * 1.732f,
        // edge 8
        elasticCubeElasticCoeff * 1.732f,
        // edge 9
        elasticCubeElasticCoeff * 1.732f,
        // edge 10
        elasticCubeElasticCoeff * 1.732f,
        // edge 11
        elasticCubeElasticCoeff * 1.732f,
        // edge 12
        elasticCubeElasticCoeff * 1.225f,
        // edge 13
        elasticCubeElasticCoeff * 1.225f,
        // edge 14
        elasticCubeElasticCoeff * 1.225f,
        // edge 15
        elasticCubeElasticCoeff * 1.225f,
        // edge 16
        elasticCubeElasticCoeff * 1.225f,
        // edge 17
        elasticCubeElasticCoeff * 1.225f,
        // edge 18
        elasticCubeElasticCoeff * 1.225f,
        // edge 19
        elasticCubeElasticCoeff * 1.225f,
        // edge 20
        elasticCubeElasticCoeff * 1.225f,
        // edge 21
        elasticCubeElasticCoeff * 1.225f,
        // edge 22
        elasticCubeElasticCoeff * 1.225f,
        // edge 23
        elasticCubeElasticCoeff * 1.225f,
        // edge 24
        elasticCubeElasticCoeff,
        // edge 25
        elasticCubeElasticCoeff,
        // edge 26
        elasticCubeElasticCoeff,
        // edge 27
        elasticCubeElasticCoeff
    };

    // d = 0.08f, nondiagonal edges scaled by 2.828/2 - damping - should be higher
    float strutDamping[28] = {
        // edge 0
        elasticCubeDampingCoeff * 1.732f,
        // edge 1
        elasticCubeDampingCoeff * 1.732f,
        // edge 2
        elasticCubeDampingCoeff * 1.732f,
        // edge 3
        elasticCubeDampingCoeff * 1.732f,
        // edge 4
        elasticCubeDampingCoeff * 1.732f,
        // edge 5
        elasticCubeDampingCoeff * 1.732f,
        // edge 6
        elasticCubeDampingCoeff * 1.732f,
        // edge 7
        elasticCubeDampingCoeff * 1.732f,
        // edge 8
        elasticCubeDampingCoeff * 1.732f,
        // edge 9
        elasticCubeDampingCoeff * 1.732f,
        // edge 10
        elasticCubeDampingCoeff * 1.732f,
        // edge 11
        elasticCubeDampingCoeff * 1.732f,
        // edge 12
        elasticCubeDampingCoeff * 1.225f,
        // edge 13
        elasticCubeDampingCoeff * 1.225f,
        // edge 14
        elasticCubeDampingCoeff * 1.225f,
        // edge 15
        elasticCubeDampingCoeff * 1.225f,
        // edge 16
        elasticCubeDampingCoeff * 1.225f,
        // edge 17
        elasticCubeDampingCoeff * 1.225f,
        // edge 18
        elasticCubeDampingCoeff * 1.225f,
        // edge 19
        elasticCubeDampingCoeff * 1.225f,
        // edge 20
        elasticCubeDampingCoeff * 1.225f,
        // edge 21
        elasticCubeDampingCoeff * 1.225f,
        // edge 22
        elasticCubeDampingCoeff * 1.225f,
        // edge 23
        elasticCubeDampingCoeff * 1.225f,
        // edge 24
        elasticCubeDampingCoeff,
        // edge 25
        elasticCubeDampingCoeff,
        // edge 26
        elasticCubeDampingCoeff,
        // edge 27
        elasticCubeDampingCoeff
    };

    unsigned int faceStrutIndices[24][3] = {
        // Front
        {0, 1, 12},
        {2, 3, 12},
        // Back
        {4, 7, 14},
        {5, 6, 14},
        // Left
        {3, 8, 15},
        {7, 10, 15},
        // Right
        {5, 9, 13},
        {1, 11, 13},
        // Top
        {2, 10, 16},
        {6, 11, 16},
        // Bottom
        {0, 8, 17},
        {4, 9, 17},
        // Other Front
        {0, 3, 18},
        {1, 2, 18},
        // Other Back
        {4, 7, 20},
        {5, 6, 20},
        // Other Left
        {7, 8, 21},
        {3, 10, 21},
        // Other Right
        {1, 9, 19},
        {5, 11, 19},
        // Other Top
        {2, 11, 22},
        {6, 10, 22},
        // Other Bottom
        {0, 9, 23},
        {4, 8, 23}
    };

    unsigned int faceVertexIndices[24][3] = {
        // Front
        {4, 5, 7},
        {4, 7, 6},
        // Back
        {0, 3, 1},
        {0, 2, 3},
        // Left
        {0, 6, 2},
        {0, 4, 6},
        // Right
        {1, 3, 5},
        {3, 7, 5},
        // Top
        {2, 6, 7},
        {2, 7, 3},
        // Bottom
        {0, 5, 4},
        {0, 1, 5},
        // Other Front
        {4, 5, 6},
        {5, 7, 6},
        // Other Back
        {0, 2, 1},
        {1, 2, 3},
        // Other Left
        {0, 4, 2},
        {2, 4, 6},
        // Other Right
        {1, 7, 5},
        {1, 3, 7},
        // Other Top
        {3, 6, 7},
        {2, 6, 3},
        // Other Bottom
        {1, 5, 4},
        {0, 1, 4}
    };

    float faceAngles[24][3] = {
        // Front
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Back
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Left
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Right
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Top
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Bottom
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Other Front
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Other Back
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Other Left 
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Other Right
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Other Top
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f},
        // Other Bottom
        {90.0f, 45.0f, 45.0f},
        {90.0f, 45.0f, 45.0f}
    };

    glm::vec3 faceNormals[24] = {
        // Front
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        // Back
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        // Left
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        // Right
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        // Top
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        // Bottom
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        // Other Front
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        // Other Back
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        // Other Left
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        // Other Right
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        // Other Top
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        // Other Bottom
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f)
    };

    float elasticCubeVelocities[24] = {
        // vertex 0
        0.0f, 0.0f, 0.0f,
        // vertex 1
        0.0f, 0.0f, 0.0f,
        // vertex 2
        0.0f, 0.0f, 0.0f,
        // vertex 3
        0.0f, 0.0f, 0.0f,
        // vertex 4
        0.0f, 0.0f, 0.0f,
        // vertex 5
        0.0f, 0.0f, 0.0f,
        // vertex 6
        0.0f, 0.0f, 0.0f,
        // vertex 7
        0.0f, 0.0f, 0.0f
    };

    // initially just gravity acting on the cubes
    float elasticCubeAccelerations[24] = {
        // vertex 0
        0.0f, -9.8f, 0.0f,
        // vertex 1
        0.0f, -9.8f, 0.0f,
        // vertex 2
        0.0f, -9.8f, 0.0f,
        // vertex 3
        0.0f, -9.8f, 0.0f,
        // vertex 4
        0.0f, -9.8f, 0.0f,
        // vertex 5
        0.0f, -9.8f, 0.0f,
        // vertex 6
        0.0f, -9.8f, 0.0f,
        // vertex 7
        0.0f, -9.8f, 0.0f
    };

    // x, y, z, tex x, tex y, norm x, norm y, norm z
    float elasticCubeVertices[64] = {
        // vertex 0
        -1.0f, 10.0f, -1.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f,
        // vertex 1
        1.0f, 10.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f,
        // vertex 2
        -1.0f, 12.0f, -1.0f, 0.0f, 1.0f, -1.0f, 1.0f, -1.0f,
        // vertex 3
        1.0f, 12.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
        // vertex 4
        -1.0f, 10.0f, 1.0f, 0.1f, 0.0f, -1.0f, -1.0f, 1.0f,
        // vertex 5
        1.0f, 10.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f,
        // vertex 6
        -1.0f, 12.0f, 1.0f, 0.0f, 0.9f, -1.0f, 1.0f, 1.0f,
        // vertex 7
        1.0f, 12.0f, 1.0f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int elasticCubeIndices[36] = {
        // Front 4, 5, 7; 4, 6, 7
        4, 5, 7,
        4, 6, 7,
        // Back 0, 1, 2; 1, 2, 3
        0, 1, 2,
        1, 2, 3,
        // Left 0, 4, 6; 0, 2, 6
        0, 4, 6,
        0, 2, 6,
        // Right 1, 3, 5; 3, 5, 7
        1, 3, 5,
        3, 5, 7,
        // Top 2, 6, 7; 2, 3, 7
        2, 6, 7,
        2, 3, 7,
        // Bottom 0, 4, 5; 0, 1, 5
        0, 4, 5,
        0, 1, 5
    };

    glm::vec3 position1[8] = {
        glm::vec3(elasticCubeVertices[0 * 8], elasticCubeVertices[0 * 8 + 1], elasticCubeVertices[0 * 8 + 2]),
        glm::vec3(elasticCubeVertices[1 * 8], elasticCubeVertices[1 * 8 + 1], elasticCubeVertices[1 * 8 + 2]),
        glm::vec3(elasticCubeVertices[2 * 8], elasticCubeVertices[2 * 8 + 1], elasticCubeVertices[2 * 8 + 2]),
        glm::vec3(elasticCubeVertices[3 * 8], elasticCubeVertices[3 * 8 + 1], elasticCubeVertices[3 * 8 + 2]),
        glm::vec3(elasticCubeVertices[4 * 8], elasticCubeVertices[4 * 8 + 1], elasticCubeVertices[4 * 8 + 2]),
        glm::vec3(elasticCubeVertices[5 * 8], elasticCubeVertices[5 * 8 + 1], elasticCubeVertices[5 * 8 + 2]),
        glm::vec3(elasticCubeVertices[6 * 8], elasticCubeVertices[6 * 8 + 1], elasticCubeVertices[6 * 8 + 2]),
        glm::vec3(elasticCubeVertices[7 * 8], elasticCubeVertices[7 * 8 + 1], elasticCubeVertices[7 * 8 + 2])
    };

    glm::vec3 velocity1[8] = {
        glm::vec3(elasticCubeVelocities[0 * 3], elasticCubeVelocities[0 * 3 + 1], elasticCubeVelocities[0 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[1 * 3], elasticCubeVelocities[1 * 3 + 1], elasticCubeVelocities[1 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[2 * 3], elasticCubeVelocities[2 * 3 + 1], elasticCubeVelocities[2 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[3 * 3], elasticCubeVelocities[3 * 3 + 1], elasticCubeVelocities[3 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[4 * 3], elasticCubeVelocities[4 * 3 + 1], elasticCubeVelocities[4 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[5 * 3], elasticCubeVelocities[5 * 3 + 1], elasticCubeVelocities[5 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[6 * 3], elasticCubeVelocities[6 * 3 + 1], elasticCubeVelocities[6 * 3 + 2]),
        glm::vec3(elasticCubeVelocities[7 * 3], elasticCubeVelocities[7 * 3 + 1], elasticCubeVelocities[7 * 3 + 2])
    };

    glm::vec3 acceleration1[8] = {
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f)
    };

    glm::vec3 position2[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    glm::vec3 velocity2[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    glm::vec3 acceleration2[8] = {
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f)
    };

    glm::vec3 position3[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    glm::vec3 velocity3[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    glm::vec3 acceleration3[8] = {
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f)
    };

    glm::vec3 position4[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    glm::vec3 velocity4[8] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    };

    glm::vec3 acceleration4[8] = {
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f),
        glm::vec3(0.0f, -9.8f, 0.0f)
    };

    glm::vec3 integratedPosition[8] = {
        position1[0],
        position1[1],
        position1[2],
        position1[3],
        position1[4],
        position1[5],
        position1[6],
        position1[7]
    };

    // Constructor
    ElasticCube(float xOffset, float yOffset, float zOffset, glm::vec3 initialVelocity) {
        numberOfVertices = 8;
        numberOfFaces = 12; // 2 tri's per cube face
        objectType = 0;
        objCenter = position1[0] + glm::vec3(1.0f, 1.0f, 1.0f);
        collisionRadius = 4.0f;
        particleMass = elasticCubeParticleMass;
        atRest = false;

        for (int i = 0; i < 12; i++) {
            for (int j = 0; j < 3; j++) {
                faceVertices[i][j] = faceVertexIndices[i][j];
            }
        }

        for (int i = 0; i < 8; i++) {
            elasticCubeVertices[i * 8] += xOffset;
            elasticCubeVertices[i * 8 + 1] += yOffset;
            elasticCubeVertices[i * 8 + 2] += zOffset;

            elasticCubeVelocities[i * 3] += initialVelocity.x;
            elasticCubeVelocities[i * 3 + 1] += initialVelocity.y;
            elasticCubeVelocities[i * 3 + 2] += initialVelocity.z;
        }

        setupCube();
    }

    void integrate(unsigned int integratorType, float stepSize, bool wasCollision, bool groundCollision) {
        if (integratorType == 0) {
            // RK4
            rk4Accelerations1(stepSize, wasCollision, groundCollision);
            rk4Accelerations2(stepSize, wasCollision, groundCollision);
            rk4Accelerations3(stepSize, wasCollision, groundCollision);
            rk4Accelerations4(stepSize, wasCollision, groundCollision);
        }
        else if (integratorType == 1) {
            // Symplectic Euler
            symplecticEuler(stepSize, wasCollision, groundCollision);
        }
        else {
            // just do RK4
            rk4Accelerations1(stepSize, wasCollision, groundCollision);
            rk4Accelerations2(stepSize, wasCollision, groundCollision);
            rk4Accelerations3(stepSize, wasCollision, groundCollision);
            rk4Accelerations4(stepSize, wasCollision, groundCollision);
        }
    }

    void symplecticEuler(float stepSize, bool wasCollision, bool groundCollision) {
        if (atRest) {
            for (int i = 0; i < 8; i++) {
                if (glm::length(velocity1[i]) > 0.1f) {
                    atRest = false;
                }
            }
        }

        for (int i = 0; i < 8; i++) {
            if (!wasCollision && !atRest) {
                position1[i] = glm::vec3(elasticCubeVertices[i * 8], elasticCubeVertices[i * 8 + 1], elasticCubeVertices[i * 8 + 2]);
                velocity1[i] = glm::vec3(elasticCubeVelocities[i * 3], elasticCubeVelocities[i * 3 + 1], elasticCubeVelocities[i * 3 + 2]);
            }
            if (!atRest) {
                acceleration1[i] = glm::vec3(0.0f, -9.8f, 0.0f);
                if (groundCollision) {
                    acceleration1[i] = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else {
                acceleration1[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }

        // calculate the accelerations first, then the velocity, then the position

        // calculating the spring and damping forces first
        for (int q = 0; q < 28; q++) {
            int g = strutVertexIndices[q][0];
            int h = strutVertexIndices[q][1];

            glm::vec3 unitVec1 = position1[h] - position1[g];
            glm::vec3 unitVec2 = position1[g] - position1[h];

            acceleration1[g] += glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: g
            acceleration1[g] += glm::normalize(unitVec1) * glm::dot((velocity1[h] - velocity1[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: g

            acceleration1[h] -= glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: h
            acceleration1[h] -= glm::normalize(unitVec1) * glm::dot((velocity1[h] - velocity1[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: h
        }

        // lift and drag forces
        for (int q = 0; q < 8; q++) {
            for (int w = 0; w < 24; w++) {
                if (faceVertexIndices[w][0] == q) {
                    int g = faceVertexIndices[w][1];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position1[g] - position1[q];
                    glm::vec3 edge2Vec = position1[h] - position1[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity1[q] + velocity1[g] + velocity1[h]) / 3.0f;

                    glm::vec3 edge3Vec = position1[h] - position1[q];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(edge1Vec, edge3Vec) / (glm::length(edge1Vec) * glm::length(edge3Vec)));
                    acceleration1[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration1[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][1] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position1[q] - position1[g];
                    glm::vec3 edge2Vec = position1[h] - position1[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity1[g] + velocity1[q] + velocity1[h]) / 3.0f;

                    glm::vec3 edge3Vec = position1[h] - position1[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position1[g] - position1[q], position1[h] - position1[q]) / (glm::length(position1[g] - position1[q]) * glm::length(position1[h] - position1[q])));
                    acceleration1[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration1[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][2] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][1];
                    glm::vec3 edge1Vec = position1[h] - position1[g];
                    glm::vec3 edge2Vec = position1[q] - position1[h];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity1[g] + velocity1[h] + velocity1[q]) / 3.0f;

                    glm::vec3 edge3Vec = position1[q] - position1[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position1[g] - position1[q], position1[h] - position1[q]) / (glm::length(position1[g] - position1[q]) * glm::length(position1[h] - position1[q])));
                    acceleration1[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration1[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
            }
        }

        glm::vec3 finalVelocity[8] = {
            velocity1[0] + acceleration1[0] * stepSize,
            velocity1[1] + acceleration1[1] * stepSize,
            velocity1[2] + acceleration1[2] * stepSize,
            velocity1[3] + acceleration1[3] * stepSize,
            velocity1[4] + acceleration1[4] * stepSize,
            velocity1[5] + acceleration1[5] * stepSize,
            velocity1[6] + acceleration1[6] * stepSize,
            velocity1[7] + acceleration1[7] * stepSize
        };

        glm::vec3 finalPosition[8] = {
            position1[0] + finalVelocity[0] * stepSize,
            position1[1] + finalVelocity[1] * stepSize,
            position1[2] + finalVelocity[2] * stepSize,
            position1[3] + finalVelocity[3] * stepSize,
            position1[4] + finalVelocity[4] * stepSize,
            position1[5] + finalVelocity[5] * stepSize,
            position1[6] + finalVelocity[6] * stepSize,
            position1[7] + finalVelocity[7] * stepSize
        };

        atRest = true;
        for (int q = 0; q < 8; q++) {
            elasticCubeVertices[q * 8] = finalPosition[q].x;
            elasticCubeVertices[q * 8 + 1] = finalPosition[q].y;
            elasticCubeVertices[q * 8 + 2] = finalPosition[q].z;

            elasticCubeVelocities[q * 3] = finalVelocity[q].x;
            elasticCubeVelocities[q * 3 + 1] = finalVelocity[q].y;
            elasticCubeVelocities[q * 3 + 2] = finalVelocity[q].z;

            if (glm::length(finalVelocity[q]) > 0.1f) {
                atRest = false;
            }
        }

        // Finding an approximate center point of the cube
        float leftmostX = finalPosition[0].x;
        float rightmostX = finalPosition[0].x;
        float leftmostY = finalPosition[0].y;
        float rightmostY = finalPosition[0].y;
        float leftmostZ = finalPosition[0].z;
        float rightmostZ = finalPosition[0].z;
        for (int i = 0; i < 8; i++) {
            // shoving in the integratedPosition update here since this is an already existing finalPosition related for loop
            integratedPosition[i] = finalPosition[i];

            if (finalPosition[i].x <= leftmostX) {
                leftmostX = finalPosition[i].x;
            }
            else if (finalPosition[i].x >= rightmostX) {
                rightmostX = finalPosition[i].x;
            }
            if (finalPosition[i].y <= leftmostY) {
                leftmostY = finalPosition[i].y;
            }
            else if (finalPosition[i].y >= rightmostY) {
                rightmostY = finalPosition[i].y;
            }
            if (finalPosition[i].z <= leftmostZ) {
                leftmostZ = finalPosition[i].z;
            }
            else if (finalPosition[i].z >= rightmostZ) {
                rightmostZ = finalPosition[i].z;
            }
        }

        objCenter = glm::vec3((leftmostX + rightmostX) / 2.0f, (leftmostY + rightmostY) / 2.0f, (leftmostZ + rightmostZ) / 2.0f);
        if (glm::abs(rightmostX - leftmostX) >= glm::abs(rightmostY - leftmostY) && glm::abs(rightmostX - leftmostX) >= glm::abs(rightmostZ - leftmostZ)) {
            collisionRadius = glm::abs(rightmostX - leftmostX);
        }
        else if (glm::abs(rightmostY - leftmostY) >= glm::abs(rightmostX - leftmostX) && glm::abs(rightmostY - leftmostY) >= glm::abs(rightmostZ - leftmostZ)) {
            collisionRadius = glm::abs(rightmostY - leftmostY);
        }
        else if (glm::abs(rightmostZ - leftmostZ) >= glm::abs(rightmostY - leftmostY) && glm::abs(rightmostZ - leftmostZ) >= glm::abs(rightmostX - leftmostX)) {
            collisionRadius = glm::abs(rightmostZ - rightmostZ);
        }
    }

    void rk4Accelerations1(float stepSize, bool wasCollision, bool groundCollision) {
        if (atRest) {
            for (int i = 0; i < 8; i++) {
                if (glm::length(velocity1[i]) > 0.2f) {
                    atRest = false;
                }
            }
        }

        for (int i = 0; i < 8; i++) {
            if (!wasCollision && !atRest) {
                position1[i] = glm::vec3(elasticCubeVertices[i * 8], elasticCubeVertices[i * 8 + 1], elasticCubeVertices[i * 8 + 2]);
                velocity1[i] = glm::vec3(elasticCubeVelocities[i * 3], elasticCubeVelocities[i * 3 + 1], elasticCubeVelocities[i * 3 + 2]);
            }
            if (!atRest) {
                acceleration1[i] = glm::vec3(0.0f, -9.8f, 0.0f);
                if (groundCollision) {
                    acceleration1[i] = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else {
                acceleration1[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }

        // spring and damping forces
        for (int q = 0; q < 28; q++) {
            int g = strutVertexIndices[q][0];
            int h = strutVertexIndices[q][1];

            glm::vec3 unitVec1 = position1[h] - position1[g];
            glm::vec3 unitVec2 = position1[g] - position1[h];

            acceleration1[g] += glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: g
            acceleration1[g] += glm::normalize(unitVec1) * glm::dot((velocity1[h] - velocity1[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: g

            // the other vertex of the spring should just be equal and opposite to the already calculated vertex
            acceleration1[h] -= glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: h
            acceleration1[h] -= glm::normalize(unitVec1) * glm::dot((velocity1[h] - velocity1[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: h
        }

        // lift and drag forces
        for (int q = 0; q < 8; q++) {
            for (int w = 0; w < 24; w++) {
                if (faceVertexIndices[w][0] == q) {
                    int g = faceVertexIndices[w][1];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position1[g] - position1[q];
                    glm::vec3 edge2Vec = position1[h] - position1[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity1[q] + velocity1[g] + velocity1[h]) / 3.0f;

                    glm::vec3 edge3Vec = position1[h] - position1[q];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(edge1Vec, edge3Vec) / (glm::length(edge1Vec) * glm::length(edge3Vec)));
                    acceleration1[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration1[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][1] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position1[q] - position1[g];
                    glm::vec3 edge2Vec = position1[h] - position1[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity1[g] + velocity1[q] + velocity1[h]) / 3.0f;

                    glm::vec3 edge3Vec = position1[h] - position1[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position1[g] - position1[q], position1[h] - position1[q]) / (glm::length(position1[g] - position1[q]) * glm::length(position1[h] - position1[q])));
                    acceleration1[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration1[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][2] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][1];
                    glm::vec3 edge1Vec = position1[h] - position1[g];
                    glm::vec3 edge2Vec = position1[q] - position1[h];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity1[g] + velocity1[h] + velocity1[q]) / 3.0f;

                    glm::vec3 edge3Vec = position1[q] - position1[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position1[g] - position1[q], position1[h] - position1[q]) / (glm::length(position1[g] - position1[q]) * glm::length(position1[h] - position1[q])));
                    acceleration1[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration1[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
            }
        }
    }

    void rk4Accelerations2(float stepSize, bool wasCollision, bool groundCollision) {
        for (int i = 0; i < 8; i++) {
            position2[i] = position1[i] + velocity1[i] * stepSize / 2.0f;
            velocity2[i] = velocity1[i] + acceleration1[i] * stepSize / 2.0f;
            if (!atRest) {
                acceleration2[i] = glm::vec3(0.0f, -9.8f, 0.0f);
                if (groundCollision) {
                    acceleration2[i] = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else {
                acceleration2[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }

        // spring and damping forces
        for (int q = 0; q < 28; q++) {
            int g = strutVertexIndices[q][0];
            int h = strutVertexIndices[q][1];

            glm::vec3 unitVec1 = position2[h] - position2[g];
            glm::vec3 unitVec2 = position2[g] - position2[h];

            acceleration2[g] += glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: g
            acceleration2[g] += glm::normalize(unitVec1) * glm::dot((velocity2[h] - velocity2[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: g

            // the other vertex of the spring should just be equal and opposite to the already calculated vertex
            acceleration2[h] -= glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: h
            acceleration2[h] -= glm::normalize(unitVec1) * glm::dot((velocity2[h] - velocity2[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: h
        }

        // lift and drag forces
        for (int q = 0; q < 8; q++) {
            for (int w = 0; w < 24; w++) {
                if (faceVertexIndices[w][0] == q) {
                    int g = faceVertexIndices[w][1];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position2[g] - position2[q];
                    glm::vec3 edge2Vec = position2[h] - position2[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity2[q] + velocity2[g] + velocity2[h]) / 3.0f;

                    glm::vec3 edge3Vec = position2[h] - position2[q];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(edge1Vec, edge3Vec) / (glm::length(edge1Vec) * glm::length(edge3Vec)));
                    acceleration2[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration2[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][1] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position2[q] - position2[g];
                    glm::vec3 edge2Vec = position2[h] - position2[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity2[g] + velocity2[q] + velocity2[h]) / 3.0f;

                    glm::vec3 edge3Vec = position2[h] - position2[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position2[g] - position2[q], position2[h] - position2[q]) / (glm::length(position2[g] - position2[q]) * glm::length(position2[h] - position2[q])));
                    acceleration2[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration2[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][2] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][1];
                    glm::vec3 edge1Vec = position2[h] - position2[g];
                    glm::vec3 edge2Vec = position2[q] - position2[h];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity2[g] + velocity2[h] + velocity2[q]) / 3.0f;

                    glm::vec3 edge3Vec = position2[q] - position2[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position2[g] - position2[q], position2[h] - position2[q]) / (glm::length(position2[g] - position2[q]) * glm::length(position2[h] - position2[q])));
                    acceleration2[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration2[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
            }
        }
    }

    void rk4Accelerations3(float stepSize, bool wasCollision, bool groundCollision) {
        for (int i = 0; i < 8; i++) {
            position3[i] = position1[i] + velocity2[i] * stepSize / 2.0f;
            velocity3[i] = velocity1[i] + acceleration2[i] * stepSize / 2.0f;
            if (!atRest) {
                acceleration3[i] = glm::vec3(0.0f, -9.8f, 0.0f);
                if (groundCollision) {
                    acceleration3[i] = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else {
                acceleration3[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }

        // spring and damping forces
        for (int q = 0; q < 28; q++) {
            int g = strutVertexIndices[q][0];
            int h = strutVertexIndices[q][1];

            glm::vec3 unitVec1 = position3[h] - position3[g];
            glm::vec3 unitVec2 = position3[g] - position3[h];

            acceleration3[g] += glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: g
            acceleration3[g] += glm::normalize(unitVec1) * glm::dot((velocity3[h] - velocity3[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: g

            // the other vertex of the spring should just be equal and opposite to the already calculated vertex
            acceleration3[h] -= glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: h
            acceleration3[h] -= glm::normalize(unitVec1) * glm::dot((velocity3[h] - velocity3[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: h
        }

        // lift and drag forces
        for (int q = 0; q < 8; q++) {
            for (int w = 0; w < 24; w++) {
                if (faceVertexIndices[w][0] == q) {
                    int g = faceVertexIndices[w][1];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position3[g] - position3[q];
                    glm::vec3 edge2Vec = position3[h] - position3[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity3[q] + velocity3[g] + velocity3[h]) / 3.0f;

                    glm::vec3 edge3Vec = position3[h] - position3[q];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(edge1Vec, edge3Vec) / (glm::length(edge1Vec) * glm::length(edge3Vec)));
                    acceleration3[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration3[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][1] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position3[q] - position3[g];
                    glm::vec3 edge2Vec = position3[h] - position3[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity3[g] + velocity3[q] + velocity3[h]) / 3.0f;

                    glm::vec3 edge3Vec = position3[h] - position3[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position3[g] - position3[q], position3[h] - position3[q]) / (glm::length(position3[g] - position3[q]) * glm::length(position3[h] - position3[q])));
                    acceleration3[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration3[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][2] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][1];
                    glm::vec3 edge1Vec = position3[h] - position3[g];
                    glm::vec3 edge2Vec = position3[q] - position3[h];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity3[g] + velocity3[h] + velocity3[q]) / 3.0f;

                    glm::vec3 edge3Vec = position3[q] - position3[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position3[g] - position3[q], position3[h] - position3[q]) / (glm::length(position3[g] - position3[q]) * glm::length(position3[h] - position3[q])));
                    acceleration3[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration3[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
            }
        }
    }

    void rk4Accelerations4(float stepSize, bool wasCollision, bool groundCollision) {
        for (int i = 0; i < 8; i++) {
            position4[i] = position1[i] + velocity3[i] * stepSize;
            velocity4[i] = velocity1[i] + acceleration3[i] * stepSize;
            if (!atRest) {
                acceleration4[i] = glm::vec3(0.0f, -9.8f, 0.0f);
                if (groundCollision) {
                    acceleration4[i] = glm::vec3(0.0f, 0.0f, 0.0f);
                }
            }
            else {
                acceleration4[i] = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }

        // spring and damping forces
        for (int q = 0; q < 28; q++) {
            int g = strutVertexIndices[q][0];
            int h = strutVertexIndices[q][1];

            glm::vec3 unitVec1 = position4[h] - position4[g];
            glm::vec3 unitVec2 = position4[g] - position4[h];

            acceleration4[g] += glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: g
            acceleration4[g] += glm::normalize(unitVec1) * glm::dot((velocity4[h] - velocity4[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: g

            // the other vertex of the spring should just be equal and opposite to the already calculated vertex
            acceleration4[h] -= glm::normalize(unitVec1) * (glm::length(unitVec1) - strutLengths[q]) * strutStiffness[q] / elasticCubeParticleMass; // spring acceleration: h
            acceleration4[h] -= glm::normalize(unitVec1) * glm::dot((velocity4[h] - velocity4[g]), glm::normalize(unitVec1)) * strutDamping[q] / elasticCubeParticleMass; // damping acceleration: h
        }

        // lift and drag forces
        for (int q = 0; q < 8; q++) {
            for (int w = 0; w < 24; w++) {
                if (faceVertexIndices[w][0] == q) {
                    int g = faceVertexIndices[w][1];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position4[g] - position4[q];
                    glm::vec3 edge2Vec = position4[h] - position4[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity4[q] + velocity4[g] + velocity4[h]) / 3.0f;

                    glm::vec3 edge3Vec = position4[h] - position4[q];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(edge1Vec, edge3Vec) / (glm::length(edge1Vec) * glm::length(edge3Vec)));
                    acceleration4[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration4[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][1] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][2];
                    glm::vec3 edge1Vec = position4[q] - position4[g];
                    glm::vec3 edge2Vec = position4[h] - position4[q];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity4[g] + velocity4[q] + velocity4[h]) / 3.0f;

                    glm::vec3 edge3Vec = position4[h] - position4[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position4[g] - position4[q], position4[h] - position4[q]) / (glm::length(position4[g] - position4[q]) * glm::length(position4[h] - position4[q])));
                    acceleration4[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration4[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
                else if (faceVertexIndices[w][2] == q) {
                    int g = faceVertexIndices[w][0];
                    int h = faceVertexIndices[w][1];
                    glm::vec3 edge1Vec = position4[h] - position4[g];
                    glm::vec3 edge2Vec = position4[q] - position4[h];
                    glm::vec3 faceNormal = glm::normalize(glm::cross(edge1Vec, edge2Vec)); // normalized normal vec

                    glm::vec3 avgVel = (velocity4[g] + velocity4[h] + velocity4[q]) / 3.0f;

                    glm::vec3 edge3Vec = position4[q] - position4[g];
                    float totalTriArea = glm::length(glm::cross(edge1Vec, edge3Vec)) / 2.0f;
                    float effectiveTriArea = totalTriArea;
                    if (glm::length(avgVel) < 0.001) {
                        // then chances are velocity is 0, and we don't want to normalize, else we get a nan error
                        effectiveTriArea *= glm::dot(faceNormal, avgVel);
                    }
                    else {
                        effectiveTriArea *= glm::dot(faceNormal, glm::normalize(avgVel));
                    }

                    glm::vec3 velNormVel = avgVel;
                    if (glm::length(avgVel) < 0.001) {
                        // if avg vel is 0, then don't want to normalize here
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, avgVel));
                    }
                    else {
                        glm::vec3 velNormVel = glm::cross(avgVel, glm::cross(faceNormal, glm::normalize(avgVel)));
                    }
                    float vertForceScalar = acos(glm::dot(position4[g] - position4[q], position4[h] - position4[q]) / (glm::length(position4[g] - position4[q]) * glm::length(position4[h] - position4[q])));
                    acceleration4[q] += -1.0f * elasticCubeDragCoeff * effectiveTriArea * avgVel * vertForceScalar / elasticCubeParticleMass; // drag force, scaled to vertex
                    acceleration4[q] += -1.0f * elasticCubeLiftCoeff * effectiveTriArea * velNormVel * vertForceScalar / elasticCubeParticleMass; // lift force scaled to vertex
                }
            }
        }

        glm::vec3 finalPosition[8] = {
                position1[0] + (velocity1[0] + 2.0f * velocity2[0] + 2.0f * velocity3[0] + velocity4[0]) * stepSize / 6.0f,
                position1[1] + (velocity1[1] + 2.0f * velocity2[1] + 2.0f * velocity3[1] + velocity4[1]) * stepSize / 6.0f,
                position1[2] + (velocity1[2] + 2.0f * velocity2[2] + 2.0f * velocity3[2] + velocity4[2]) * stepSize / 6.0f,
                position1[3] + (velocity1[3] + 2.0f * velocity2[3] + 2.0f * velocity3[3] + velocity4[3]) * stepSize / 6.0f,
                position1[4] + (velocity1[4] + 2.0f * velocity2[4] + 2.0f * velocity3[4] + velocity4[4]) * stepSize / 6.0f,
                position1[5] + (velocity1[5] + 2.0f * velocity2[5] + 2.0f * velocity3[5] + velocity4[5]) * stepSize / 6.0f,
                position1[6] + (velocity1[6] + 2.0f * velocity2[6] + 2.0f * velocity3[6] + velocity4[6]) * stepSize / 6.0f,
                position1[7] + (velocity1[7] + 2.0f * velocity2[7] + 2.0f * velocity3[7] + velocity4[7]) * stepSize / 6.0f
        };

        // Finding an approximate center point of the cube
        float leftmostX = finalPosition[0].x;
        float rightmostX = finalPosition[0].x;
        float leftmostY = finalPosition[0].y;
        float rightmostY = finalPosition[0].y;
        float leftmostZ = finalPosition[0].z;
        float rightmostZ = finalPosition[0].z;
        for (int i = 0; i < 8; i++) {
            // shoving in the integratedPosition update here since this is an already existing finalPosition related for loop
            integratedPosition[i] = finalPosition[i];

            if (finalPosition[i].x <= leftmostX) {
                leftmostX = finalPosition[i].x;
            }
            else if (finalPosition[i].x >= rightmostX) {
                rightmostX = finalPosition[i].x;
            }
            if (finalPosition[i].y <= leftmostY) {
                leftmostY = finalPosition[i].y;
            }
            else if (finalPosition[i].y >= rightmostY) {
                rightmostY = finalPosition[i].y;
            }
            if (finalPosition[i].z <= leftmostZ) {
                leftmostZ = finalPosition[i].z;
            }
            else if (finalPosition[i].z >= rightmostZ) {
                rightmostZ = finalPosition[i].z;
            }
        }

        objCenter = glm::vec3((leftmostX + rightmostX) / 2.0f, (leftmostY + rightmostY) / 2.0f, (leftmostZ + rightmostZ) / 2.0f);
        if (glm::abs(rightmostX - leftmostX) >= glm::abs(rightmostY - leftmostY) && glm::abs(rightmostX - leftmostX) >= glm::abs(rightmostZ - leftmostZ)) {
            collisionRadius = glm::abs(rightmostX - leftmostX);
        }
        else if (glm::abs(rightmostY - leftmostY) >= glm::abs(rightmostX - leftmostX) && glm::abs(rightmostY - leftmostY) >= glm::abs(rightmostZ - leftmostZ)) {
            collisionRadius = glm::abs(rightmostY - leftmostY);
        }
        else if (glm::abs(rightmostZ - leftmostZ) >= glm::abs(rightmostY - leftmostY) && glm::abs(rightmostZ - leftmostZ) >= glm::abs(rightmostX - leftmostX)) {
            collisionRadius = glm::abs(rightmostZ - rightmostZ);
        }

        glm::vec3 finalVelocity[8] = {
            velocity1[0] + (acceleration1[0] + 2.0f * acceleration2[0] + 2.0f * acceleration3[0] + acceleration4[0]) * stepSize / 6.0f,
            velocity1[1] + (acceleration1[1] + 2.0f * acceleration2[1] + 2.0f * acceleration3[1] + acceleration4[1]) * stepSize / 6.0f,
            velocity1[2] + (acceleration1[2] + 2.0f * acceleration2[2] + 2.0f * acceleration3[2] + acceleration4[2]) * stepSize / 6.0f,
            velocity1[3] + (acceleration1[3] + 2.0f * acceleration2[3] + 2.0f * acceleration3[3] + acceleration4[3]) * stepSize / 6.0f,
            velocity1[4] + (acceleration1[4] + 2.0f * acceleration2[4] + 2.0f * acceleration3[4] + acceleration4[4]) * stepSize / 6.0f,
            velocity1[5] + (acceleration1[5] + 2.0f * acceleration2[5] + 2.0f * acceleration3[5] + acceleration4[5]) * stepSize / 6.0f,
            velocity1[6] + (acceleration1[6] + 2.0f * acceleration2[6] + 2.0f * acceleration3[6] + acceleration4[6]) * stepSize / 6.0f,
            velocity1[7] + (acceleration1[7] + 2.0f * acceleration2[7] + 2.0f * acceleration3[7] + acceleration4[7]) * stepSize / 6.0f
        };

        atRest = true;
        for (int q = 0; q < 8; q++) {
            elasticCubeVertices[q * 8] = finalPosition[q].x;
            elasticCubeVertices[q * 8 + 1] = finalPosition[q].y;
            elasticCubeVertices[q * 8 + 2] = finalPosition[q].z;

            elasticCubeVelocities[q * 3] = finalVelocity[q].x;
            elasticCubeVelocities[q * 3 + 1] = finalVelocity[q].y;
            elasticCubeVelocities[q * 3 + 2] = finalVelocity[q].z;

            if (glm::length(finalVelocity[q]) > 0.2f) {
                atRest = false;
            }
        }
    }

    void resetPositionAndVelocity() {
        for (int i = 0; i < 8; i++) {
            elasticCubeVertices[i * 8] = position1[i].x;
            elasticCubeVertices[i * 8 + 1] = position1[i].y;
            elasticCubeVertices[i * 8 + 2] = position1[i].z;

            elasticCubeVelocities[i * 3] = velocity1[i].x;
            elasticCubeVelocities[i * 3 + 1] = velocity1[i].y;
            elasticCubeVelocities[i * 3 + 2] = velocity1[i].z;
        }
    }

private:
    void setupCube() {
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(elasticCubeVertices), elasticCubeVertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elasticCubeIndices), elasticCubeIndices, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

