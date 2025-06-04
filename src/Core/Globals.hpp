#pragma once
#include "imgui.h"
#include <map>
#include <shader.h>
#include <vector>
#include "../Components/Structures.hpp"

struct GlobalState
{
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    unsigned int drawCalls = 0;
    std::map<std::string, float> time_map; 
};
extern GlobalState global_state;

struct EngineData 
{
    // Engine Data
    Shader shaders[32];
    std::vector<Texture> textures;
};
extern EngineData engine_data;
