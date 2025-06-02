
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.hpp"


void RenderSystem::Render(WindowManager &windowManager, entt::registry &registry, float fov, Camera camera)
{
    auto meshesView = registry.view<Transform, ModelData, RenderingData>();
    auto lightsView = registry.view<Transform, Light>();

    Light light1;
    glm::vec3 light1Position;

    lightsView.each([&](auto& transform, auto& light) {
        float time = glfwGetTime();
        transform.position = glm::vec3(sin(time), transform.position.y, cos(time));
        light1 = light;
        light1Position = transform.position;
    });

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)windowManager.width / (float)windowManager.height, 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

    for (auto entity: meshesView) {
        auto &transform = meshesView.get<Transform>(entity);
        auto &modelData = meshesView.get<ModelData>(entity);
        auto &renderingData = meshesView.get<RenderingData>(entity);

        // activate shader
        renderingData.shader->use();

        if (registry.any_of<Texture>(entity)) {
            auto& texture = registry.get<Texture>(entity);

            // bind textures on corresponding texture units
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture.textureID);
            renderingData.shader->setInt("texture1", 0); // Showing that 'texture1' uses GL_TEXTURE0
        }

        // for lighting stuff
        renderingData.shader->setVec3("viewPos", camera.Position);

        renderingData.shader->setVec3("objectColor", renderingData.material.albedo);

        renderingData.shader->setVec3("material.ambient", renderingData.material.ambient);
        renderingData.shader->setVec3("material.diffuse", renderingData.material.diffuse);
        renderingData.shader->setVec3("material.specular", renderingData.material.specular);
        renderingData.shader->setFloat("material.shininess", renderingData.material.shininess);

        renderingData.shader->setVec3("light.ambient", light1.ambient);
        renderingData.shader->setVec3("light.diffuse", light1.diffuse);
        renderingData.shader->setVec3("light.specular", light1.specular);
        renderingData.shader->setVec3("light.position", light1Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        renderingData.shader->setMat4("projection", projectionMatrix);
        // camera/view transformation
        renderingData.shader->setMat4("view", viewMatrix);

        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 RotationMatrix = glm::mat4_cast(transform.rotation);
        modelMatrix *= RotationMatrix; // Rotate
        modelMatrix = glm::translate(modelMatrix, transform.position); // Translate
        modelMatrix = glm::scale(modelMatrix, transform.scale); // Scale


        renderingData.shader->setMat4("model", modelMatrix);

        glBindVertexArray(modelData.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    };

};

struct VectorFloatHasher {
    std::size_t operator()(std::vector<float> const& vec) const {
    std::size_t seed = vec.size();
    for(float f : vec) {
        int x = (int)(f*1000);
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
    }
};

struct VertexBuffers {
    unsigned int VAO, VBO;
};

void RenderSystem::BindVertexArray(entt::registry &registry) {

    auto view = registry.view<ModelData>();

    VectorFloatHasher hasher;

    // I use an unordered map to check if an identical vertex vector has been assigned VAO and VBOs previously
    // If so, these are immediately assigned to the corresponding ModelData variables
    std::unordered_map<size_t, VertexBuffers> m;


    view.each([&](ModelData& modelData) {

        size_t modelHash = hasher(modelData.vertices);
        if (m.find(modelHash) != m.end()) {
            VertexBuffers v = m[modelHash];
            modelData.VAO = v.VAO;
            modelData.VBO = v.VBO;
            return;
        }


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

        m[modelHash] = {modelData.VAO, modelData.VBO };

    });

    
}