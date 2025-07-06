
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Core/GameState.hpp"

#include "RenderSystem.hpp"
void RenderInstanced(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);


void RenderSystem::Render(entt::registry &registry, float fov, Camera camera, glm::vec2 projectionRatio)
{   
    
    auto lightsView = registry.view<Transform, ModelWrapper, LightTag>();
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), projectionRatio.x / projectionRatio.y, 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
    glm::mat4 skyBoxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // Remove translation


    glBindBuffer(GL_UNIFORM_BUFFER, state->uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), 16, glm::value_ptr(camera.Position));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    // Dir = 64, Point = 64, Spot = 80 (Without extra transform)
    // Dir = 64, Point = 80, Spot = 96 (With extra transform)
    // 64 + 80 * 4 + 96 * 4 = 768 + vec3 numLights (16) = 784

    static_assert(sizeof(DirectionalLight) == 64);
    static_assert(sizeof(PointLight) == 64);
    static_assert(sizeof(SpotLight) == 80);
    unsigned int dirLight = 0, pointLights = 0, spotLights = 0;
    unsigned int max_lights = 4;
    glBindBuffer(GL_UNIFORM_BUFFER, state->uboLights);
    for (auto entity : lightsView) {
        auto& transform = registry.get<Transform>(entity);
        auto& modelWrapper = registry.get<ModelWrapper>(entity);

        if (DirectionalLight *d = registry.try_get<DirectionalLight>(entity)) {
            if (dirLight >= 1) continue;
            glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, d);
            dirLight++;
        } else if (PointLight *p = registry.try_get<PointLight>(entity)) {
            if (pointLights >= max_lights) continue;
            glBufferSubData(GL_UNIFORM_BUFFER, 64+(pointLights)*80, 16, glm::value_ptr(transform.position));
            glBufferSubData(GL_UNIFORM_BUFFER, 64+(pointLights)*80+16, 64, p);
            pointLights++;
        } else if (SpotLight *s = registry.try_get<SpotLight>(entity)) {
            if (spotLights >= max_lights) continue;
            glBufferSubData(GL_UNIFORM_BUFFER, 64+max_lights*80+(spotLights)*96, 16, glm::value_ptr(transform.position));
            glBufferSubData(GL_UNIFORM_BUFFER, 64+max_lights*80+(spotLights)*96+16, 80, s);
            spotLights++;
        }
        Shader lightshader = state->engine_data.shaders["light"];
        lightshader.use();

        glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        modelMatrix = glm::translate(modelMatrix, transform.position); // Translate
        modelMatrix *= transform.rotation; // Rotate
        modelMatrix = glm::scale(modelMatrix, transform.scale); // Scale
        lightshader.setMat4("model", modelMatrix);

        modelWrapper.model.Draw(lightshader, -1);
    }
    glm::vec3 numLights = glm::vec3(dirLight, pointLights, spotLights);
    glBufferSubData(GL_UNIFORM_BUFFER, 768, 16, glm::value_ptr(numLights));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
    glDisable(GL_CULL_FACE);
    auto modelsView = registry.view<Transform, RenderingData, ModelWrapper>();
    modelsView.each([&](auto& transform, auto& renderingData, auto& modelWrapper) {
        renderingData.shader->use();

        glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        modelMatrix = glm::translate(modelMatrix, transform.position); // Translate
        modelMatrix *= transform.rotation; // Rotate
        modelMatrix = glm::scale(modelMatrix, transform.scale); // Scale
        renderingData.shader->setMat4("model", modelMatrix);

        renderingData.shader->setFloat("shininess", 32);
        

        modelWrapper.model.Draw(*renderingData.shader, -1);
    });
    glEnable(GL_CULL_FACE); 


    
    // Render Instanced Meteorites
    // unsigned int amount = 10000;
    // // draw meteorites
    // state->engine_data.shaders["instanced"].use();

    // for(unsigned int i = 0; i < state->rock->meshes.size(); i++)
    // {
    //     glBindVertexArray(state->rock->meshes[i].VAO);
    //     glDrawElementsInstanced(
    //         GL_TRIANGLES, state->rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
    //     );
    // }

    // Render Skybox

    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content    
    state->engine_data.shaders["skybox"].use();
    state->engine_data.shaders["skybox"].setMat4("projection", projectionMatrix);
    state->engine_data.shaders["skybox"].setMat4("view", skyBoxView);
    // glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(state->skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, state->skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS); // set depth function back to default

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

void RenderSystem::BindVertexArray(entt::registry &registry, bool reloadVBOs) {

    auto view = registry.view<ModelData>();

    VectorFloatHasher hasher;

    // I use an unordered map to check if an identical vertex vector has been assigned VAO and VBOs previously
    // If so, these are immediately assigned to the corresponding ModelData variables
    std::unordered_map<size_t, VertexBuffers> m;


    view.each([&](ModelData& modelData) {

        size_t modelHash = hasher(modelData.vertices);
        if (m.find(modelHash) != m.end() && !reloadVBOs) {
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        m[modelHash] = {modelData.VAO, modelData.VBO };

    });

    
}