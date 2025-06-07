#pragma once 

#include "WindowManager.hpp"
#include <entt/entt.hpp>
#include "../Systems/RenderSystem.hpp"
#include <camera.h>
#include "GameState.hpp"
#include <model.h>


class Game {
    public:
        static void Init(WindowManager *gameWindowManager, GameState *gameState);
        static const int Run(ImGuiContext *hostContext);
        static int Events(float deltaTime);
        static void Render(Model &model);
        static void Shutdown();
};