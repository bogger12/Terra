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
// Base Alignment _ Alignment
struct DirectionalLight {
    glm::vec4 direction; // 16 0
    glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // 16 16
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 16 32
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // 16 48
}; // Total 64

struct PointLight {
    glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // 16 0
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 16 16
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // 16 32
    // Values for point light attenuation
    float constant; // 4 48
    float linear; // 4 52
    float quadratic; // 4 56
    float _pad;
}; // Total 60 (64)

struct SpotLight {
    glm::vec4 direction; // 16 0
    glm::vec4 ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); // 16 16
    glm::vec4 diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // 16 32
    glm::vec4 specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // 16 48
    // Values for Spotlight
    float cutOff; // 4 64
    float outerCutOff; // 4 68
    float _pad1;
    float _pad2;
}; // Total 72 (80)