///
/// main.cpp
/// EnttPong
///
/// Refer to LICENSE.txt for more details.
///

#include <ctime>
#include <iostream>

#include "Core/Game.hpp"

int main(int argsc, char *argsv[])
{
    // Seeds RNG.
    // std::srand(std::time(nullptr));

    try
    {
        Game game("OpenGL Game", 640, 480);
        return game.Run();
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