#pragma once
#include "imgui.h"
#include <map>
#include <shader.h>
#include <vector>
#include "../Components/Structures.hpp"
#include "WindowManager.hpp"
#include <entt/entt.hpp>
#include <camera.h>

struct EngineData 
{
    // Engine Data
    std::vector<Shader> shaders;
    std::vector<Texture> textures;
};

struct GameState
{
    entt::registry m_registry;
    entt::dispatcher m_dispatcher;

    EngineData engine_data;

    Camera camera;


    // GUI State
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    unsigned int drawCalls = 0;

    // Rendering/Input variables
    float fov = 45.0f;
    bool firstMouse = true;
    float lastX = 800.0f / 2.0;
    float lastY = 600.0 / 2.0;
    bool useMouse = true;
};
// Pointer to state used by game lib
extern GameState *state; 

extern std::map<std::string, float> time_map; // For time testing

