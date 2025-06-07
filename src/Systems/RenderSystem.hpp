#pragma once

#include "../Core/WindowManager.hpp"
#include "../Components/All.hpp"
#include "../Components/Lights.hpp"
#include <entt/entt.hpp>
#include <camera.h>

class RenderSystem {
    public:
        RenderSystem() = default;
        static void Render(WindowManager &windowManager, entt::registry &registry, float fov, Camera camera);
        static void BindVertexArray(entt::registry &registry, bool reloadVBOs=false);
};