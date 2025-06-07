///
/// main.cpp
/// EnttPong
///
/// Refer to LICENSE.txt for more details.
///

#include <ctime>
#include <iostream>
#include <dlfcn.h>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/Game.hpp"
#include "Core/WindowManager.hpp"
#include "Core/GameState.hpp"



int load_shared_lib();
const char* libPath = "libTerraGame.dylib";
bool instanceCreated = false;

WindowManager gameWindowManager = WindowManager();
GameState gameState;


int main(int argsc, char *argsv[])
{

    // Seeds RNG.
    // std::srand(std::time(nullptr));
    int w = 800, h = 600;
    const char *windowName = "Terra Engine";
    try
    {
        // Game game("OpenGL Game", 800, 600);
        gameWindowManager.Create(w, h, windowName);
        while(true) {
            load_shared_lib();
        }
        // return game.Run();
    }
    catch (const std::exception &e)
    {
        // Error message printed to console, dont need to reprint exception,
        // just wait for player input.

        std::cin.get();
        return EXIT_FAILURE;
    }
    catch (...)
    {
        // Error message printed to console, dont need to reprint exception,
        // just wait for player input.

        std::cin.get();
        return EXIT_FAILURE;
    }
}


int load_shared_lib() {
    
    void* handle = dlopen(libPath, RTLD_NOW);
    if (!handle) {
        std::cerr << "Failed to load game library: " << dlerror() << std::endl;
        return 1;
    }

    typedef void (*CreateGameFn)(WindowManager *gameWindowManager, GameState *gameState, bool instanceCreated);
    typedef void (*RunGameFn)(ImGuiContext *hostContext);
    CreateGameFn createGame = (CreateGameFn)dlsym(handle, "CreateGame");
    RunGameFn runGame = (RunGameFn)dlsym(handle, "RunGame");

    if (!createGame) {
        std::cerr << "Failed to find CreateGame symbol: " << dlerror() << std::endl;
        dlclose(handle);
        return 1;
    }

    createGame(&gameWindowManager, &gameState, instanceCreated);  // Call into game
    gameWindowManager.InitialiseGUI();
    ImGui_ImplGlfw_InstallCallbacks(gameWindowManager.GetWindow());
    instanceCreated = true;
    runGame(ImGui::GetCurrentContext());
    gameWindowManager.ShutdownGUI();

    dlclose(handle);
    return 0;
}