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
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    // Values for point light attenuation
    glm::vec3 direction;

    void SetShaderValues(Shader *shader, glm::vec3 position) {
        shader->setVec3("dirLight.ambient", ambient);
        shader->setVec3("dirLight.diffuse", diffuse);
        shader->setVec3("dirLight.specular", specular);
        shader->setVec3("dirLight.direction", direction);
    };
};

struct PointLight {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    // Values for point light attenuation
    float constant;
    float linear;
    float quadratic;

    void SetShaderValues(Shader *shader, glm::vec3 position, int i) {
        std::ostringstream lightUniform; lightUniform << "pointLights[" << i << "].";
        shader->setVec3(std::string(lightUniform.str() + "position"), position);
        shader->setVec3(std::string(lightUniform.str() + "ambient"), ambient);
        shader->setVec3(std::string(lightUniform.str() + "diffuse"), diffuse);
        shader->setVec3(std::string(lightUniform.str() + "specular"), specular);
        shader->setFloat(std::string(lightUniform.str() + "constant"), constant);
        shader->setFloat(std::string(lightUniform.str() + "linear"), linear);
        shader->setFloat(std::string(lightUniform.str() + "quadratic"), quadratic);
    };
};

struct SpotLight {
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // darken diffuse light a bit
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    // Values for Spotlight
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    void SetShaderValues(Shader *shader, glm::vec3 position, int i) {
        std::ostringstream lightUniform; lightUniform << "spotLights[" << i << "].";
        shader->setVec3(std::string(lightUniform.str() + "position"), position);
        shader->setVec3(std::string(lightUniform.str() + "ambient"), ambient);
        shader->setVec3(std::string(lightUniform.str() + "diffuse"), diffuse);
        shader->setVec3(std::string(lightUniform.str() + "specular"), specular);
        shader->setVec3(std::string(lightUniform.str() + "direction"), direction);
        shader->setFloat(std::string(lightUniform.str() + "cutOff"), glm::cos(glm::radians(cutOff)));
        shader->setFloat(std::string(lightUniform.str() + "outerCutOff"), glm::cos(glm::radians(outerCutOff)));
    };
};