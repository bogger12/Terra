#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>

namespace Math {

    void DecomposeMatrix(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale) {
        pos = m[3];
        for(int i = 0; i < 3; i++)
            scale[i] = glm::length(glm::vec3(m[i]));
        const glm::mat3 rotMtx(
            glm::vec3(m[0]) / scale[0],
            glm::vec3(m[1]) / scale[1],
            glm::vec3(m[2]) / scale[2]);
        rot = glm::quat_cast(rotMtx);
    }
};