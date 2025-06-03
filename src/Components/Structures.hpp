#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


struct Material {
    glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 32;

    bool operator==(const Material& other) const {
        return albedo == other.albedo && 
            ambient == other.ambient && 
            diffuse == other.diffuse && 
            specular == other.specular && 
            shininess == other.shininess;
    }
    bool operator!=(const Material& other) { return !(*this == other); }

}; 