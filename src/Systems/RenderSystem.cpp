
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "RenderSystem.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/io.hpp>

void RenderSystem::Render(Window &window, entt::registry &registry, float fov, Camera camera)
{
    auto view = registry.view<Transform, Texture, ModelData>();

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)window.width / (float)window.height, 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

    view.each([&](const auto& transform, auto& texture, auto& modelData) {

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.textureID);

#ifdef DEBUG // To hot reload shaders - adds texture thing
        // activate shader
        modelData.shader->use();
        modelData.shader->setInt("texture1", 0); // Showing that 'texture1' uses GL_TEXTURE0

        // for lighting stuff TODO:
        modelData.shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        modelData.shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
#endif

        // pass projection matrix to shader (note that in this case it could change every frame)
        modelData.shader->setMat4("projection", projectionMatrix);
        // camera/view transformation
        modelData.shader->setMat4("view", viewMatrix);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 RotationMatrix = glm::mat4_cast(transform.rotation);
        modelMatrix *= RotationMatrix; // Rotate
        modelMatrix = glm::scale(modelMatrix, transform.scale); // Scale
        modelMatrix = glm::translate(modelMatrix, transform.position); // Translate

        modelData.shader->setMat4("model", modelMatrix);

        glBindVertexArray(modelData.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    });

};

void RenderSystem::BindVertexArray(entt::registry &registry) {

    auto view = registry.view<ModelData>();


    view.each([&](auto& modelData) {
        // unsigned int VBO, VAO;

        // activate shader
        modelData.shader->use();
        modelData.shader->setInt("texture1", 0); // Showing that 'texture1' uses GL_TEXTURE0

        // for lighting stuff TODO:
        modelData.shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        modelData.shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        glGenVertexArrays(1, &modelData.VAO);
        glGenBuffers(1, &modelData.VBO);

        glBindVertexArray(modelData.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO);
        glBufferData(GL_ARRAY_BUFFER, modelData.vertices.size() * sizeof(float), &modelData.vertices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    });

    
}