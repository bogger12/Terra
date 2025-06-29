#pragma once

#include <glm/glm.hpp>
#include <shader.h>


// struct Light {
//     glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
//     glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // darken diffuse light a bit
//     glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
//     virtual void SetShaderValues(Shader *shader, glm::vec3 position, int n=1);
// };

struct LightTag {};

struct DirectionalLight {
    glm::vec4 direction;
    glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // darken diffuse light a bit
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct PointLight {
    glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // darken diffuse light a bit
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Values for point light attenuation
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    glm::vec4 direction;
    glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // darken diffuse light a bit
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Values for Spotlight
    float cutOff;
    float outerCutOff;
};