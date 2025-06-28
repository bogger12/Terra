#pragma once

#include <glad/glad.h>


class Primitives {
    public:
        Primitives() = default;
        static void SetupQuadVAO(unsigned int *quadVAO);
        static void SetupSkyboxVAO(unsigned int *skyboxVAO);
};