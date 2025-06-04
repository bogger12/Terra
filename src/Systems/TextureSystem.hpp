#pragma once
#include "../Components/Structures.hpp"

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
        static void LoadTextures(std::vector<Texture> &textures);
};

unsigned int LoadTextureFromPath(std::string texture_path, int &width, int &height, int &nrChannels, GLint internalFormat = GL_RGB);
