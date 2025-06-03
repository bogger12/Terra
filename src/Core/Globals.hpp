#pragma once
#include "imgui.h"
#include <map>

struct GlobalState
{
    bool show_demo_window = false;
    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    unsigned int drawCalls = 0;
    std::map<std::string, float> time_map; 
};
extern GlobalState global_state;
