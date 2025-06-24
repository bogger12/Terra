#pragma once

#include "Structures.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <shader.h>
#include <vector>
#include <model.h>


// ------------------
// Components
// ------------------

struct Transform {
    glm::vec3 position;
    glm::mat4 rotation;
    glm::vec3 scale;
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

struct ModelWrapper {
    Model model;
};