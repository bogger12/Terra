#pragma once
#include "imgui.h"
#include <imgui_stdlib.h>
#include <entt/entt.hpp>
#include "../Core/Globals.hpp"

class GUISystem {
    public:
        GUISystem() = default;
        static void DrawSideBar(entt::registry &registry, GlobalState *global_state, void (*reload_shaders)());
};