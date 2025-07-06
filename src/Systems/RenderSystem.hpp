#pragma once

#include "../Core/WindowManager.hpp"
#include "../Components/Components.hpp"
#include "../Components/Lights.hpp"
#include <entt/entt.hpp>
#include <camera.h>

class RenderSystem {
    public:
        RenderSystem() = default;
        static void Render(entt::registry &registry, float fov, Camera camera, glm::vec2 projectionRatio);
        static void BindVertexArray(entt::registry &registry, bool reloadVBOs=false);
};