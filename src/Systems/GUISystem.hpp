#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#define IM_VEC2_CLASS_EXTRA \
    constexpr ImVec2(glm::vec2& f) : x(f.x), y(f.y) {} \
    operator glm::vec2() const { return glm::vec2(x, y); }

#include "imgui.h"
#include "imgui_internal.h"
#include <imgui_stdlib.h>
#include <entt/entt.hpp>
#include "../Core/GameState.hpp"
#include "../Core/WindowManager.hpp"


class GUISystem {
    public:
        GUISystem() = default;
        static void DrawSideBar(entt::registry &registry, GameState *state, EngineData *engine_data, WindowManager &windowManager, void (*reload_shaders)(), void (*GameInit)(WindowManager *windowManager, GameState *gameState));

};