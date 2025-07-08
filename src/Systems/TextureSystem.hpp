#pragma once
#include "../Components/Structures.hpp"

#include <platformGL.h>
#include <stb_image.h>

#include <string>
#include <iostream>
#include <entt/entt.hpp>

class TextureSystem {
    public:
        TextureSystem() = default;
        static void LoadTextures(std::vector<MaterialTexture> &textures);
        static unsigned int LoadCubemap(std::vector<std::string> faces, std::string directory, GLint format = GL_RGB);
};

unsigned int LoadTextureFromPath(std::string texture_path, int &width, int &height, int &nrChannels, GLint internalFormat = GL_RGB);
