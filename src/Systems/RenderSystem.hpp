#pragma once

#include "../Core/WindowManager.hpp"
#include "../Components/All.hpp"
#include <entt/entt.hpp>
#include <camera.h>

class RenderSystem {
    public:
        RenderSystem() = default;
        void Render(WindowManager &windowManager, entt::registry &registry, float fov, Camera camera);
        void BindVertexArray(entt::registry &registry);
};