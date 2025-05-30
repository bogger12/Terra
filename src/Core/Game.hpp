#pragma once 

#include "Window.hpp"
#include <entt/entt.hpp>
#include "../Systems/RenderSystem.hpp"
#include <camera.h>

class Game {
    public:
        Game(std::string title, const int w, const int h);
        const int Run();
        void Events(float deltaTime);
        void Render();
    private:
        Window window;

        entt::registry m_registry;

        entt::dispatcher m_dispatcher;

        RenderSystem renderSystem;
};