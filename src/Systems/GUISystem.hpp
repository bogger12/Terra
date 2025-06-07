#pragma once
#include "imgui.h"
#include <imgui_stdlib.h>
#include <entt/entt.hpp>
#include "../Core/GameState.hpp"
#include "../Core/WindowManager.hpp"


class GUISystem {
    public:
        GUISystem() = default;
        static void DrawSideBar(entt::registry &registry, GameState *state, EngineData *engine_data, WindowManager &windowManager, void (*reload_shaders)());
};