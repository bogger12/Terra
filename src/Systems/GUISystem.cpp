#include "GUISystem.hpp"
#include "../Components/All.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/TextureSystem.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

bool MaterialTextureNode(const char *label, MaterialTexture &texture);
bool TextureNode(const char *label, Texture &texture);


glm::vec3 ExtractEulerAngles(const glm::mat4& matrix) {
    glm::quat q = glm::quat_cast(glm::mat3(matrix));
    glm::vec3 euler = glm::eulerAngles(q);
    return glm::degrees(euler);
}
glm::mat4 EulerAnglesToMat4(const glm::vec3& euler) {
    return glm::mat4_cast(glm::quat(glm::radians(euler)));
}

void GUISystem::DrawSideBar(entt::registry &registry, GameState *state, EngineData *engine_data, WindowManager &windowManager, void (*reload_shaders)(), void (*GameInit)(WindowManager *windowManager, GameState *gameState))
{
    ImGuiIO &io = ImGui::GetIO();
    // Sidebad Window
    {
        // static int counter = 0;

        static float windowWidth = 300.0f;

        // Set Window Parameters
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - windowWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(windowWidth, io.DisplaySize.y)); // Optional: full height
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Window Bg Color
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Window Border Color

        ImGui::Begin("SideBar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove); // Create a window called "Hello, world!" and append into it.
        windowWidth = ImGui::GetWindowSize().x;

        const char *buildType = "(Alpha)";
#ifdef DEBUG
        buildType = "Hot Reloadable";
#endif
        ImGui::Text("Terra Engine v0.0.1 %s", buildType);                       // Display some text (you can use a format strings too)
        ImGui::Dummy(ImVec2(0.0f, 20.0f));
        ImGui::Checkbox("Demo Window", &state->show_demo_window); // Edit bools storing our window open/close state
        ImGui::ColorEdit3("Clear color", (float *)&state->clear_color); // Edit 3 floats representing a color

        // if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
        //     counter++;
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);
        static bool vsyncOn = true;
        std::string vSyncText = std::string("VSync: ") + (vsyncOn ? "On" : "Off");
        if (ImGui::Button(vSyncText.c_str())) { vsyncOn = !vsyncOn; windowManager.ChangeVSync(vsyncOn); };

        if (ImGui::Button("Reload Shaders")) reload_shaders(); 

        if (ImGui::Button("Reinit State")) GameInit(&windowManager, state);

        float aspect = (float)windowManager.width / (float)windowManager.height;
        ImGui::Image((ImTextureID)(intptr_t)state->renderTexture, ImVec2(windowWidth * 0.95f, windowWidth * 0.95f / aspect), {0, 1}, {1, 0});



        ImGui::Text("Draw Calls: %u", state->drawCalls);
        



        ImGui::Text("Hot reload text");

        ImGui::Text("Average %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        for (auto const& [timer, time] : time_map)
        {
            ImGui::Text("%s : %.3f ms", timer.c_str(), time);
        }
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Text("Entities View");
        ImGui::Spacing();
        auto allEntities = registry.view<entt::entity>();
        ImGui::BeginChild("Entities", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));

        for (auto entity : allEntities) {
            uint32_t id = static_cast<uint32_t>(entity);
            bool hasTransform = registry.any_of<Transform>(entity);
            bool hasModelData = registry.any_of<ModelData>(entity);
            bool hasRenderingData = registry.any_of<RenderingData>(entity);
            bool hasPointLight = registry.any_of<PointLight>(entity);
            bool hasModelWrapper = registry.any_of<ModelWrapper>(entity);
            bool hasSpotLight = registry.any_of<SpotLight>(entity);
            bool hasDirectionalLight = registry.any_of<DirectionalLight>(entity);


            ImGui::PushID(id);
            std::string entityType;
            if (hasPointLight || hasSpotLight || hasDirectionalLight) entityType = "Light";
            else if (hasModelWrapper) entityType = "Mesh";
            else entityType = "Empty";
            std::ostringstream name; name << entityType << " " << id;
            std::string nameStr = name.str();

            if (ImGui::TreeNode(nameStr.c_str()))
            {   
                // Transform
                if (hasTransform && ImGui::TreeNode("Transform")) {
                    auto &transform = registry.get<Transform>(entity);
                    ImGui::DragFloat3("Position", glm::value_ptr(transform.position));
                    static glm::vec3 eulerRotation = ExtractEulerAngles(transform.rotation);
                    if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerRotation))) {
                        transform.rotation = EulerAnglesToMat4(eulerRotation);
                    };
                    ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale));
                    ImGui::TreePop();
                }

                // ModelData
                if (hasModelData && ImGui::TreeNode("ModelData"))
                {
                    auto &modelData = registry.get<ModelData>(entity);
                    ImGui::Text("VBO: %d, VAO: %d", modelData.VBO, modelData.VAO);
                    for (int i = 0; i < 36; i ++) {
                        ImGui::PushID(i);
                        if (ImGui::DragFloat3("", &modelData.vertices[i*5])) {
                            RenderSystem::BindVertexArray(registry);
                        };
                        ImGui::PopID();
                    };
                    ImGui::TreePop();
                }

                // RenderingData
                if (hasRenderingData && ImGui::TreeNode("RenderingData"))
                {
                    auto &renderingData = registry.get<RenderingData>(entity);
                    ImGui::Text("Shader ID: %u", renderingData.shader->ID);
                    if (ImGui::TreeNode("Material")) {
                        ImGui::ColorEdit3("Albedo", glm::value_ptr(renderingData.material.albedo));
                        ImGui::ColorEdit3("Diffuse", glm::value_ptr(renderingData.material.diffuse));
                        if (MaterialTextureNode("Diffuse Texture", *renderingData.material.diffuseMap)) {
                            TextureSystem::LoadTextures(engine_data->textures); // Reload on path change
                        };
                        ImGui::ColorEdit3("Specular", glm::value_ptr(renderingData.material.specular));
                        if (MaterialTextureNode("Specular Texture", *renderingData.material.specularMap)) {
                            TextureSystem::LoadTextures(engine_data->textures); // Reload on path change
                        };
                        ImGui::DragFloat("Shininess", &renderingData.material.shininess);
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }

                // ModelWrapper
                if (hasModelWrapper && ImGui::TreeNode("ModelWrapper"))
                {
                    auto &modelWrapper = registry.get<ModelWrapper>(entity);

                    if (ImGui::TreeNode("Model")) {
                        // Calculate Total Vertices:
                        unsigned int verticesCount = 0;
                        for (Mesh m : modelWrapper.model.meshes) { verticesCount += m.vertices.size(); }
                        // Slow to iterate throguh twice
                        ImGui::TextColored(ImVec4(0, 255, 0, 1), "%u Vertices Total", verticesCount);
                        ImGui::TextColored(ImVec4(0, 255, 0, 1), "%lu Meshes", modelWrapper.model.meshes.size());

                        for ( Mesh m : modelWrapper.model.meshes ) {
                            ImGui::PushID(m.VAO);

                            ImGui::Text("%lu Vertices", m.vertices.size());
                            ImGui::Text("%lu Textures", m.textures.size());
                            unsigned int tcount = 0;
                            for (Texture t : m.textures) {
                                ImGui::PushID(tcount);
                                if (TextureNode(t.type.c_str(), t)) {
                                    // Nothing
                                }
                                tcount++;
                                ImGui::PopID();
                            }
                            ImGui::PopID();
                        }
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                

                // Light
                if (hasPointLight && ImGui::TreeNode("PointLight"))
                {
                    auto &pointLight = registry.get<PointLight>(entity);

                    if (ImGui::TreeNode("Light Material")) {
                        ImGui::ColorEdit3("Ambient", glm::value_ptr(pointLight.ambient));
                        ImGui::ColorEdit3("Diffuse", glm::value_ptr(pointLight.diffuse));
                        ImGui::ColorEdit3("Specular", glm::value_ptr(pointLight.specular));
                        ImGui::TreePop();
                    }
                    ImGui::DragFloat("Constant", &pointLight.constant);
                    ImGui::DragFloat("Linear", &pointLight.linear);
                    ImGui::DragFloat("Quadratic", &pointLight.quadratic);
                    ImGui::TreePop();
                }

                if (hasSpotLight && ImGui::TreeNode("SpotLight"))
                {
                    auto &spotLight = registry.get<SpotLight>(entity);

                    if (ImGui::TreeNode("Light Material")) {
                        ImGui::ColorEdit3("Ambient", glm::value_ptr(spotLight.ambient));
                        ImGui::ColorEdit3("Diffuse", glm::value_ptr(spotLight.diffuse));
                        ImGui::ColorEdit3("Specular", glm::value_ptr(spotLight.specular));
                        ImGui::TreePop();
                    }
                    ImGui::DragFloat3("Direction", glm::value_ptr(spotLight.direction));
                    ImGui::DragFloat("Cutoff", &spotLight.cutOff);
                    ImGui::DragFloat("Outer Cutoff", &spotLight.outerCutOff);
                    ImGui::TreePop();
                }
                if (hasDirectionalLight && ImGui::TreeNode("Directional Light"))
                {
                    auto &dirLight = registry.get<DirectionalLight>(entity);

                    if (ImGui::TreeNode("Light Material")) {
                        ImGui::ColorEdit3("Ambient", glm::value_ptr(dirLight.ambient));
                        ImGui::ColorEdit3("Diffuse", glm::value_ptr(dirLight.diffuse));
                        ImGui::ColorEdit3("Specular", glm::value_ptr(dirLight.specular));
                        ImGui::TreePop();
                    }
                    ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.direction));
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
            ImGui::PopID();

        }
        ImGui::EndChild();

        ImGui::End();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
};


bool MaterialTextureNode(const char *label, MaterialTexture &texture) {
    MaterialTexture initTexture = texture;
    if (ImGui::TreeNode(label))
    {
        ImGui::InputText("Path", &texture.texture_path);
        ImGui::Text("size = %d x %d", texture.width, texture.height);
        ImGui::Text("textureID: %u", texture.textureID);
        if (ImGui::TreeNode("Color Mode")) {
            if (ImGui::Selectable("GL_RGB", texture.internalFormat==GL_RGB)) { texture.internalFormat = GL_RGB; }
            if (ImGui::Selectable("GL_RGBA", texture.internalFormat==GL_RGBA)) { texture.internalFormat = GL_RGBA; }
            ImGui::TreePop();
        }
        float aspect = (float)texture.width / (float)texture.height;
        ImGui::Image((ImTextureID)(intptr_t)texture.textureID, ImVec2(100.0f * aspect, 100.0f), {0, 1}, {1, 0});

        ImGui::TreePop();
    }
    return (initTexture != texture); 
};

bool TextureNode(const char *label, Texture &texture) {
    Texture initTexture = texture;
    if (ImGui::TreeNode(label))
    {
        ImGui::InputText("Path", &texture.path);
        // ImGui::Text("size = %d x %d", texture., texture.height);
        ImGui::Text("textureID: %u", texture.id);
        ImGui::Image((ImTextureID)(intptr_t)texture.id, ImVec2(100.0f, 100.0f), {0, 1}, {1, 0});

        ImGui::TreePop();
    }
    return (initTexture.path != texture.path); 
}