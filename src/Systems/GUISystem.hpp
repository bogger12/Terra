#pragma once
#include "imgui.h"
#include <imgui_stdlib.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <entt/entt.hpp>

struct GlobalState
{
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    std::map<std::string, float> time_map; 
};

class GUISystem {
    public:
        GUISystem() = default;
        static void DrawSideBar(entt::registry &registry, GlobalState *global_state, void (*reload_shaders)());
};