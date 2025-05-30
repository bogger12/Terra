#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <shader.h>
#include <vector>

#ifndef ASSET_DIR
#define ASSET_DIR "../assets"
#endif

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
    unsigned int VBO = 0, VAO = 0;
    Shader shader = Shader(ASSET_DIR "/shaders/vert.glsl", ASSET_DIR "/shaders/frag.glsl");
};