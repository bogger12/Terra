#pragma once
#include <glm/glm.hpp>


namespace Physics {
    static bool InsideBBox2D(glm::vec2 pos, glm::vec4 bbox) {
        bool inx = pos.x >= bbox.x && pos.x <= bbox.z;
        bool iny = pos.y >= bbox.y && pos.y <= bbox.w;
        return inx && iny;
    };
};