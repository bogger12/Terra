#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


struct Material {
    glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32;
}; 