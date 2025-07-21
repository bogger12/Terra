#include "GUISystem.hpp"
#include "../Components/Components.hpp"
#include "../Systems/RenderSystem.hpp"
#include "../Systems/TextureSystem.hpp"
#include "../Editor/Gizmos.hpp"
#include "../Editor/Editor.hpp"
#include "../Utilities/Graphics.hpp"
#include "../Utilities/Debug.hpp"
#include "../Core/OSMethods.hpp"



using namespace entt::literals;

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

void DrawComponent(Transform &transform) {
    if (ImGui::TreeNode("Transform")) {
        ImGui::DragFloat3("Position", glm::value_ptr(transform.position));
        static glm::vec3 eulerRotation = ExtractEulerAngles(transform.rotation);
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerRotation))) {
            transform.rotation = EulerAnglesToMat4(eulerRotation);
        };
        ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale));
        ImGui::TreePop();
    }
}

void DrawComponent(ModelData &modelData) {
    if (ImGui::TreeNode("ModelData")) {
        ImGui::Text("VBO: %d, VAO: %d", modelData.VBO, modelData.VAO);
        for (int i = 0; i < modelData.vertices.size(); i ++) {
            ImGui::PushID(i);
            if (ImGui::DragFloat3("", &modelData.vertices[i*5])) {
                // RenderSystem::BindVertexArray(registry);
            };
            ImGui::PopID();
        };
        ImGui::TreePop();
    }
}

void DrawComponent(RenderingData &renderingData) {
    if (ImGui::TreeNode("RenderingData")) {
        ImGui::Text("Shader ID: %u", renderingData.shader->ID);
        // if (ImGui::TreeNode("Material")) {
        //     ImGui::ColorEdit3("Albedo", glm::value_ptr(renderingData.material.albedo));
        //     ImGui::ColorEdit3("Diffuse", glm::value_ptr(renderingData.material.diffuse));
        //     if (MaterialTextureNode("Diffuse Texture", *renderingData.material.diffuseMap)) {
        //         // TextureSystem::LoadTextures(engine_data->textures); // Reload on path change
        //     };
        //     ImGui::ColorEdit3("Specular", glm::value_ptr(renderingData.material.specular));
        //     if (MaterialTextureNode("Specular Texture", *renderingData.material.specularMap)) {
        //         // TextureSystem::LoadTextures(engine_data->textures); // Reload on path change
        //     };
        //     ImGui::DragFloat("Shininess", &renderingData.material.shininess);
        //     ImGui::TreePop();
        // }
        ImGui::TreePop();
    }
}

void DrawComponent(ModelWrapper &modelWrapper) {
    if (ImGui::TreeNode("ModelWrapper")) {
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
}

void DrawComponent(PointLight &pointLight) {
    if (ImGui::TreeNode("PointLight")) {

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
}

void DrawComponent(SpotLight &spotLight) {
    if (ImGui::TreeNode("SpotLight"))
    {
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
}

void DrawComponent(DirectionalLight &dirLight) {
    if (ImGui::TreeNode("Directional Light")) {

        if (ImGui::TreeNode("Light Material")) {
            ImGui::ColorEdit3("Ambient", glm::value_ptr(dirLight.ambient));
            ImGui::ColorEdit3("Diffuse", glm::value_ptr(dirLight.diffuse));
            ImGui::ColorEdit3("Specular", glm::value_ptr(dirLight.specular));
            ImGui::TreePop();
        }
        ImGui::DragFloat3("Direction", glm::value_ptr(dirLight.direction));
        ImGui::TreePop();
    }
}

void DrawAddWindow() {

}


void GUISystem::Initialise() {
    ImGuiIO &io = ImGui::GetIO();
    static std::string iniPath = getExecutableDir().string() + "/imgui.ini";
    io.IniFilename = iniPath.c_str();
    std::cout << "imgui file: " << io.IniFilename << std::endl;
}


void GUISystem::DrawSideBar(entt::registry &registry, GameState *state, EngineData *engine_data, void (*reload_shaders)(), void (*GameInit)(WindowManager *window, GameState *gameState))
{
    ImGuiIO &io = ImGui::GetIO();
    // Sidebar Window
    {

        static float windowWidth = 300.0f;
        static bool showAddWindow = false;
        static entt::entity selected_entity = entt::null;


        auto allEntities = registry.view<entt::entity>();



        if (state->show_demo_window) ImGui::ShowDemoWindow(); // Show demo window! :)


        bool opt_fullscreen = true;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
        }
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, ImVec4(0, 0, 0, 0)); // Fully transparent
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("Main Dockspace", nullptr, window_flags);

        ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

        ImGui::DockSpace(dockspace_id, ImVec2(0.0f,0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
        ImGui::PopStyleColor();  // Pop ImGuiCol_DockingEmptyBg
        ImGui::PopStyleVar(2);  // Pop ImGuiCol_DockingEmptyBg


        // Check if layout has already been initialised
        ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
        static bool layout_initialized = (node && node->ChildNodes[0] != nullptr);


        if (!layout_initialized) {
            layout_initialized = true;

            ImGuiID dock_main_id = dockspace_id;
            ImGuiID left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, &left_id, &dock_main_id);
            ImGuiID right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.3f, &right_id, &dock_main_id);
            ImGuiID bottom_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, &bottom_id, &dock_main_id);
            
            ImGui::DockBuilderDockWindow("Tools", left_id);
            ImGui::DockBuilderDockWindow("Logs", bottom_id);
            ImGui::DockBuilderDockWindow("Inspector", right_id);
            ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

            ImGui::DockBuilderFinish(dockspace_id);
        }

        static bool autoScroll = true; // You can make this a toggle too
        
        ImGui::Begin("Logs");
        ImGui::Text("%s", Debug::ReadLog().c_str());
        if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f); // Scroll to bottom
        }
        ImGui::End();


        // Editor View Window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport", nullptr);
        ImVec2 editorViewSize = ImGui::GetContentRegionAvail();

        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 absMin = ImVec2(windowPos.x + contentMin.x, windowPos.y + contentMin.y);
        ImVec2 absMax = ImVec2(windowPos.x + contentMax.x, windowPos.y + contentMax.y);

        glm::ivec2 size = glm::vec2(editorViewSize)*windowManager->contentScale; // Mult by 2 due to retina screen
        // Change size of buffers
        if (state->editorViewBuffer.size != size) {
            Graphics::ChangeFramebufferSize(&state->postProcessBuffer, size);
            Graphics::ChangeFramebufferSize(&state->editorViewBuffer, size);
            Editor::viewRect = glm::vec4(absMin.x, absMin.y, absMax.x, absMax.y);
            // state->editorViewRect = glm::vec4(0,0, size.x, size.y);
        }

        ImGui::Image((ImTextureID)(intptr_t)state->editorViewBuffer.renderTexture, ImVec2(editorViewSize), {0, 1}, {1, 0});
        ImGui::End();
        ImGui::PopStyleVar();


        // Sidebar Window

        // ImGui::SetNextWindowSize(ImVec2(windowWidth, io.DisplaySize.y));
        // ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Window Bg Color
        // ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Window Border Color

        ImGui::Begin("Tools", nullptr);
        windowWidth = ImGui::GetContentRegionAvail().x;

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
        if (ImGui::Button(vSyncText.c_str())) { vsyncOn = !vsyncOn; WindowManager::ChangeVSync(vsyncOn); };

        if (ImGui::Button("Reload Shaders")) reload_shaders(); 

        if (ImGui::Button("Reinit State")) GameInit(windowManager, state);

        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) ImGui::Text(" DockingEnable");
        
        
        float aspect = editorViewSize.x / editorViewSize.y;
        ImGui::Image((ImTextureID)(intptr_t)state->postProcessBuffer.renderTexture, ImVec2(windowWidth * 0.95, windowWidth * 0.95 / aspect), {0, 1}, {1, 0});

        ImGui::Text("Draw Calls: %u", state->drawCalls);
        

        ImGui::Text("Average %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        for (auto const& [timer, time] : time_map) ImGui::Text("%s : %.3f ms", timer.c_str(), time);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Text("Entities View");
        ImGui::Spacing();
        ImGui::BeginChild("Entities", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f));

        for (auto entity : allEntities) {
            uint32_t id = static_cast<uint32_t>(entity);
            bool hasTransform = registry.any_of<Transform>(entity);
            bool hasModelData = registry.any_of<ModelData>(entity);
            bool hasRenderingData = registry.any_of<RenderingData>(entity);
            bool hasModelWrapper = registry.any_of<ModelWrapper>(entity);
            bool hasPointLight = registry.any_of<PointLight>(entity);
            bool hasSpotLight = registry.any_of<SpotLight>(entity);
            bool hasDirectionalLight = registry.any_of<DirectionalLight>(entity);


            ImGui::PushID(id);
            std::string entityType;
            if (hasPointLight) entityType = "Point Light";
            else if (hasSpotLight) entityType = "Spotlight";
            else if (hasDirectionalLight) entityType = "Directional Light";
            else if (hasModelWrapper) entityType = "Mesh";
            else entityType = "Empty";

            std::string entityLabel = entityType + " " + std::to_string(id);
            if (ImGui::Selectable(entityLabel.c_str(), selected_entity == entity)) {
                selected_entity = entity;
                Gizmos::globalGizmo.transform = &registry.get<Transform>(selected_entity);
                
                Debug::Log("selected " + entityLabel);
            };

            ImGui::PopID();

        }
        if (ImGui::Button("+")) showAddWindow = true;
        ImGui::EndChild();

        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::Text("This is the Inspector window");
        if (selected_entity != entt::null) {
            bool hasTransform = registry.any_of<Transform>(selected_entity);
            bool hasModelData = registry.any_of<ModelData>(selected_entity);
            bool hasRenderingData = registry.any_of<RenderingData>(selected_entity);
            bool hasModelWrapper = registry.any_of<ModelWrapper>(selected_entity);
            bool hasPointLight = registry.any_of<PointLight>(selected_entity);
            bool hasSpotLight = registry.any_of<SpotLight>(selected_entity);
            bool hasDirectionalLight = registry.any_of<DirectionalLight>(selected_entity);

            if (hasTransform) DrawComponent(registry.get<Transform>(selected_entity));
            if (hasModelData) DrawComponent(registry.get<ModelData>(selected_entity));
            if (hasRenderingData) DrawComponent(registry.get<RenderingData>(selected_entity));
            if (hasModelWrapper) DrawComponent(registry.get<ModelWrapper>(selected_entity));

            // Light
            if (hasPointLight) DrawComponent(registry.get<PointLight>(selected_entity));
            if (hasSpotLight) DrawComponent(registry.get<SpotLight>(selected_entity));
            if (hasDirectionalLight) DrawComponent(registry.get<DirectionalLight>(selected_entity));
        }

        ImGui::End();




        if (showAddWindow) {
            ImGui::Begin("Add New Entity", &showAddWindow);

            

            for(const auto& [typeName, createComponent] : Components::ComponentFactories) {
                ImGui::Text("%s", typeName.c_str());
                std::string buttonText = "Add " + typeName;
                // if (ImGui::Button(buttonText.c_str())) {
                //     createComponent(registry, new_entity); // Add Component to entity
                // }
            }   
            

            // ImGui::Checkbox("Transform", &componentFlags[typeid(Transform)]);
            // if (componentFlags[typeid(Transform)]) ImGui::Text("TRansofrm");

            ImGui::End();
        }
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
        ImGui::Text("textureID: %u", texture.id);
        ImGui::Image((ImTextureID)(intptr_t)texture.id, ImVec2(100.0f, 100.0f), {0, 1}, {1, 0});
        ImGui::TreePop();
    }
    return (initTexture.path != texture.path); 
}