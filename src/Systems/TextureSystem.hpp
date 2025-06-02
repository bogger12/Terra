#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <string>
#include <iostream>
#include <filesystem>
#include <entt/entt.hpp>

class TextureSystem {
    public:
        TextureSystem() = default;
        static void LoadTextures(entt::registry &registry);
};

unsigned int LoadTextureFromPath(std::string texture_path, int &width, int &height, int &nrChannels, GLint internalFormat = GL_RGB);
