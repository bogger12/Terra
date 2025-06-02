#include <unordered_map>
#include "TextureSystem.hpp"
#include "../Components/All.hpp"

struct TextureDescriptor {
    unsigned int textureID;
    int width = 0, height = 0, nrChannels = 0;
};

void TextureSystem::LoadTextures(entt::registry &registry)
{
    std::unordered_map<std::string, TextureDescriptor> m;
    
    auto textures = registry.view<Texture>();

    textures.each([&](Texture &tex) {
        TextureDescriptor td;
        
        if (m.find(tex.texture_path) != m.end()) { // If exists in map
            td = m[tex.texture_path];
            std::cout << "Texture already exists, using textureID " <<  td.textureID << std::endl;
        } else {
            td.textureID = LoadTextureFromPath(tex.texture_path, td.width, td.height, td.nrChannels, tex.internalFormat);
            m[tex.texture_path] = td;
            std::cout << "creating textureID " <<  td.textureID << std::endl;

        }
        tex.textureID = td.textureID;
        tex.height = td.height; tex.width = td.width;
        tex.nrChannels = td.nrChannels;
    });
}

unsigned int LoadTextureFromPath(std::string texture_path, int &width, int &height, int &nrChannels, GLint internalFormat)
{
    unsigned int textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps

    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded textures on the y-axis.
    unsigned char *data = stbi_load(std::filesystem::path(texture_path).c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}