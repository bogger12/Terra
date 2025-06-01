#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <shader.h>
#include <vector>

struct Transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

struct Texture {
    // std::string texture_path;
    unsigned int textureID;
    int height = 0;
    int width = 0; 
    int nrChannels = 0;
};

struct ModelData {
    std::vector<float> vertices;
    // std::vector<unsigned int> indices;
    Shader *shader;
    unsigned int VBO = 0, VAO = 0;
};

struct Light {
    float intensity = 1; // unused
};