
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.hpp"


void RenderSystem::Render(WindowManager &windowManager, entt::registry &registry, float fov, Camera camera)
{
    auto meshesView = registry.view<Transform, ModelData>();
    auto lightsView = registry.view<Transform, Light>();

    glm::vec3 lightPosition;

    lightsView.each([&](auto& transform, auto& light) {
        float time = glfwGetTime();
        transform.position = glm::vec3(sin(time), 1.0 , cos(time));
        lightPosition = transform.position;
    });

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)windowManager.width / (float)windowManager.height, 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

    for (auto entity: meshesView) {
        auto &transform = meshesView.get<Transform>(entity);
        auto &modelData = meshesView.get<ModelData>(entity);

        // activate shader
        modelData.shader->use();

        if (registry.any_of<Texture>(entity)) {
            auto& texture = registry.get<Texture>(entity);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.textureID);
            modelData.shader->setInt("texture1", 0); // Showing that 'texture1' uses GL_TEXTURE0
        }

        // for lighting stuff
        modelData.shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        modelData.shader->setVec3("viewPos", camera.Position);

        modelData.shader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        modelData.shader->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        modelData.shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        modelData.shader->setFloat("material.shininess", 32.0f);

        modelData.shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        modelData.shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        modelData.shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        modelData.shader->setVec3("light.position", lightPosition);

        // pass projection matrix to shader (note that in this case it could change every frame)
        modelData.shader->setMat4("projection", projectionMatrix);
        // camera/view transformation
        modelData.shader->setMat4("view", viewMatrix);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 RotationMatrix = glm::mat4_cast(transform.rotation);
        modelMatrix *= RotationMatrix; // Rotate
        modelMatrix = glm::translate(modelMatrix, transform.position); // Translate
        modelMatrix = glm::scale(modelMatrix, transform.scale); // Scale


        modelData.shader->setMat4("model", modelMatrix);

        glBindVertexArray(modelData.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    };

};

void RenderSystem::BindVertexArray(entt::registry &registry) {

    auto view = registry.view<ModelData>();


    view.each([&](auto& modelData) {
        // unsigned int VBO, VAO;

        glGenVertexArrays(1, &modelData.VAO);
        glGenBuffers(1, &modelData.VBO);

        glBindVertexArray(modelData.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO);
        glBufferData(GL_ARRAY_BUFFER, modelData.vertices.size() * sizeof(float), &modelData.vertices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

    });

    
}