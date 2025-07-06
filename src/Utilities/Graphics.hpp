#pragma once

#include "../Core/WindowManager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <shader.h>


struct Framebuffer {
    unsigned int fbo = 0;
    unsigned int renderTexture = 0;
    glm::ivec2 size;
    Shader* shader;
};

struct RenderFramebuffer : Framebuffer  {
    unsigned int depthTexture = 0;
};

class Graphics {
    public:
        static void SetupFramebuffer(Framebuffer* fb, glm::ivec2 size, Shader *shader);
        static void SetupFramebuffer(RenderFramebuffer* fb, glm::ivec2 size, Shader *shader);
        static void BindFrameBuffer(Framebuffer* fb);
        static void BindFrameBuffer(RenderFramebuffer* fb);
        static void ChangeFramebufferSize(Framebuffer* fb, glm::ivec2 size);
        static void ChangeFramebufferSize(RenderFramebuffer* fb, glm::ivec2 size);
        static void DrawFrame(Framebuffer *fb, unsigned int VAO);
        static void DrawFrame(RenderFramebuffer *fb, unsigned int VAO);
};