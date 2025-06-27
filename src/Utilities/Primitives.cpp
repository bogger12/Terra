
#include "Primitives.hpp"



void Primitives::SetupQuadVAO(unsigned int *quadVAO) {

    float quadVertices[] = {
        // positions          // texture Coords 
         -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         1.0f, 1.0f,  0.0f,  1.0f, 1.0f,
         -1.0f, 1.0f,  0.0f,  0.0f, 1.0f,

         -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         1.0f, 1.0f,  0.0f,  1.0f, 1.0f
    };

    unsigned int vbo;

    glGenVertexArrays(1, quadVAO);
    glGenBuffers(1, &vbo);

    glBindVertexArray(*quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}