#pragma once

#include "Structures.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <shader.h>
#include <vector>

GlobalState global_state;

struct Transform {
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

struct Texture {
    std::string texture_path;
    GLint internalFormat = GL_RGB;
    unsigned int textureID = 0;
    int height = 0;
    int width = 0; 
    int nrChannels = 0;
};

struct ModelData {
    std::vector<float> vertices;
    // std::vector<unsigned int> indices;
    unsigned int VBO = 0, VAO = 0;
};

struct RenderingData {
    Shader *shader;
    Material material;
};

struct Light {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    // float intensity = 1; // unused
};