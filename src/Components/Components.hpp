#pragma once

#include "Structures.hpp"
#include "Lights.hpp"
#include "../Core/OSMethods.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <shader.h>
#include <vector>
#include <model.h>
#include <map>
#include <entt/entt.hpp>


// ------------------
// Components
// ------------------

struct Transform {
    glm::vec3 position = glm::vec3(0.0,0.0,0.0);
    glm::mat4 rotation = glm::mat4(0.0);
    glm::vec3 scale = glm::vec3(1.0,1.0,1.0);;
};

struct ModelData {
    std::vector<float> vertices = {};
    // std::vector<unsigned int> indices;
    unsigned int VBO = 0, VAO = 0;
};

struct RenderingData {
    Shader *shader = nullptr;
    // Material material;
};

struct ModelWrapper {
    Model model = Model(asset("/models/default/cube.obj"));
};

class Components {

    public:

        using ComponentAdder = std::function<void(entt::registry&, entt::entity&)>;

        static const std::map<std::string, ComponentAdder> ComponentFactories;

};