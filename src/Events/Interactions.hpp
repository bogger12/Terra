#pragma once
#include <glm/glm.hpp>

struct Click {
    bool clicked = false;
    bool isFirstDown = false;
    glm::vec2 clickCurrentLoc;
    glm::vec2 clickStartLoc;

    void MouseDown(const glm::vec2& location) {
        isFirstDown = !clicked;
        clicked = true;
        clickStartLoc = isFirstDown ? location : clickStartLoc;
        clickCurrentLoc = location;
    }
    void MouseUp() {
        isFirstDown = false;
        clicked = false;
        clickCurrentLoc = clickStartLoc = glm::vec2(0.0f);
    }
};