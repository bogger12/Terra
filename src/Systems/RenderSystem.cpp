
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Core/GameState.hpp"

#include "RenderSystem.hpp"


void RenderSystem::BindFrameBuffer(WindowManager &windowManager, unsigned int *fbo, unsigned int *renderTexture, unsigned int *depthTexture) {
    // unsigned int fbo;
    glGenFramebuffers(1, fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

    glGenTextures(1, renderTexture);
    glBindTexture(GL_TEXTURE_2D, *renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowManager.width, windowManager.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    glGenTextures(1, depthTexture);
    glBindTexture(GL_TEXTURE_2D, *depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowManager.width, windowManager.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *renderTexture, 0);  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *depthTexture, 0);  
    
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);


    // unsigned int rbo;
    // glGenRenderbuffers(1, &rbo);
    // glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowManager.width, windowManager.height);  
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // // Attach the renderbuffer object to the depth and stencil attachment of the framebuffer
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // glDeleteFramebuffers(1, &fbo); 
    // glDeleteRenderbuffers(1, &rbo);


    // We get the texture and fbo out of this - rbo is discarded once allocated
    
}


void RenderSystem::Render(WindowManager &windowManager, entt::registry &registry, float fov, Camera camera)
{   
    
    auto lightsView = registry.view<Transform, LightTag>();

    // Move lights
    // lightsView.each([&](auto& transform) {
    //     float time = glfwGetTime();
    //     transform.position = glm::vec3(sin(time), transform.position.y, cos(time));
    // });

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), (float)windowManager.width / (float)windowManager.height, 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);

    glBindBuffer(GL_UNIFORM_BUFFER, state->uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for (auto entity : lightsView) {
        if (DirectionalLight *d = registry.try_get<DirectionalLight>(entity)) {
            glBindBuffer(GL_UNIFORM_BUFFER, state->uboDirLight);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(DirectionalLight), d);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }
    }

    glm::mat4 skyBoxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));  


    auto modelsView = registry.view<Transform, RenderingData, ModelWrapper>();
    modelsView.each([&](auto& transform, auto& renderingData, auto& modelWrapper) {
        // auto &transform = meshesView.get<Transform>(entity);
        // auto &modelWrapper = meshesView.get<ModelWrapper>(entity);
        glUseProgram(renderingData.shader->ID);

        renderingData.shader->setVec3("viewPos", camera.Position);

        glm::mat4 modelMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        modelMatrix = glm::translate(modelMatrix, transform.position); // Translate
        modelMatrix *= transform.rotation; // Rotate
        modelMatrix = glm::scale(modelMatrix, transform.scale); // Scale        
        renderingData.shader->setMat4("model", modelMatrix);
        
        // // LIGHTS

        // int pointLightCount = 0; int spotLightCount = 0;
        // for (auto entity : lightsView) {
        //     Transform &transform = registry.get<Transform>(entity);
        //     if (PointLight *p = registry.try_get<PointLight>(entity)) p->SetShaderValues(renderingData.shader, transform.position, pointLightCount++);
        //     else if (DirectionalLight *d = registry.try_get<DirectionalLight>(entity)) d->SetShaderValues(renderingData.shader, transform.position);
        //     else if (SpotLight *s = registry.try_get<SpotLight>(entity)) s->SetShaderValues(renderingData.shader, transform.position, spotLightCount++);
        // }
        // // renderingData.shader->setInt("numPointLights", pointLightCount);
        // // renderingData.shader->setInt("numSpotLights", spotLightCount);

        renderingData.shader->setFloat("shininess", 32);
        

        modelWrapper.model.Draw(*renderingData.shader, true);
    });

    // Render Skybox

    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content    
    state->engine_data.shaders[4].use();
    state->engine_data.shaders[4].setMat4("projection", projectionMatrix);
    state->engine_data.shaders[4].setMat4("view", skyBoxView);
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