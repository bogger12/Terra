#pragma once

#include <glad/glad.h>
#include <string>


class Primitives {
    public:
        Primitives() = default;
        static void SetupQuadVAO(unsigned int *quadVAO);
        static void SetupSkyboxVAO(unsigned int *skyboxVAO);
        static void SetupUniformBuffer(unsigned int &ubo, unsigned int size, unsigned int shaderID, std::string uniformBlockName, unsigned int uniformBlockBinding);
};