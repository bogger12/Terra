#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>


struct Texture {
    std::string texture_path;
    GLint internalFormat = GL_RGB;
    unsigned int textureID = 0;
    int height = 0;
    int width = 0; 
    int nrChannels = 0;

    bool operator==(const Texture& other) const {
        return texture_path == other.texture_path &&
            internalFormat == other.internalFormat &&
            textureID == other.textureID;
    }
    bool operator!=(const Texture& other) { return !(*this == other); }
};

struct Material {
    glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);
    Texture *diffuseMap;
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32;

    bool operator==(const Material& other) const {
        return albedo == other.albedo && 
            diffuseMap == other.diffuseMap && 
            specular == other.specular && 
            shininess == other.shininess;
    }
    bool operator!=(const Material& other) { return !(*this == other); }

}; 