#pragma once
#include "imgui.h"
#include <imgui_stdlib.h>
#include <entt/entt.hpp>
#include "../Core/Globals.hpp"
#include "../Core/WindowManager.hpp"


class GUISystem {
    public:
        GUISystem() = default;
        static void DrawSideBar(entt::registry &registry, GlobalState *global_state, EngineData *engine_data, WindowManager &windowManager, void (*reload_shaders)());
};