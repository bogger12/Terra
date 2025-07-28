#include <random>

#include "Game.hpp"
#include "../Components/Components.hpp"
#include "../Events/KeyDown.hpp"
#include "../Events/Interactions.hpp"
#include <camera.h>
#include <terraphysics.h>
#include <terramath.h>
#include "../Systems/RenderSystem.hpp"
#include "../Editor/Gizmos.hpp"
#include "../Editor/Editor.hpp"
#include "../Systems/TextureSystem.hpp"
#include "../Utilities/Primitives.hpp"
#include "../Utilities/Graphics.hpp"
#include "../Systems/GUISystem.hpp"
#include "GameState.hpp"
#include "OSMethods.hpp"

#include <model.h>



void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void reload_shaders();
void test_performance_entities(entt::registry &m_registry, int numObjects, float positionRange[2], float scaleRange[2], Model *model, Shader *shader);

WindowManager *windowManager;
GameState* state;

// For time testing
std::map<std::string, float> time_map;



// NEEDED FOR DYNAMIC LINKING SYMBOL LOOKUP
#ifdef DEBUG
extern "C" void CreateGame(WindowManager* gameWindow, GameState *gameState, bool isCreated) {
    state = gameState;
    windowManager = gameWindow;
    WindowManager::InitialiseGlad();

    windowManager->SetCallbacks(framebuffer_size_callback, mouse_callback, scroll_callback);
    // glfwMakeContextCurrent(gameWindowManager->GetWindow());

    if (!isCreated) {
        Game::Init(gameWindow, gameState);
    }
}

extern "C" int RunGame(ImGuiContext *hostContext) {
    glfwMakeContextCurrent(windowManager->GetWindow()); // `window` must be a valid GLFWwindow*
    WindowManager::InitialiseGlad(); // Initialise GLAD function pointers again as they get removed during hot reload

    return Game::Run(hostContext);
}
#endif

// Game Code

void Game::Init(WindowManager* gameWindow, GameState *gameState)
{
#ifndef DEBUG
    state = gameState;
    windowManager = gameWindow;
    windowManager->SetCallbacks(framebuffer_size_callback, mouse_callback, scroll_callback);
#endif

    // Clear entities
    state->m_registry.clear();
    

    
    std::unordered_map<std::string, Shader> &shaders = state->engine_data.shaders;
    // Setting Shaders
    // Shader default_shader = Shader(asset("/shaders/vert_lit.vert"), asset("/shaders/frag_lit.frag"));
    shaders["light"] = Shader(asset("/shaders/light.vert"), asset("/shaders/light.frag"));
    shaders["model"] = Shader(asset("/shaders/model_litdir.vert"), asset("/shaders/model_litdir.frag"));
    shaders["instanced"] = Shader(asset("/shaders/model_litdirinstanced.vert"), asset("/shaders/model_litdirinstanced.frag"));
    shaders["postprocess"] = Shader(asset("/shaders/postprocessquad.vert"), asset("/shaders/postprocessquad.frag"));
    shaders["editorview"] = Shader(asset("/shaders/editorviewquad.vert"), asset("/shaders/editorviewquad.frag"));
    shaders["rayquad"] = Shader(asset("/shaders/rayquad.vert"), asset("/shaders/rayquad.frag"));
    shaders["gizmoplane"] = Shader(asset("/shaders/gizmoplane.vert"), asset("/shaders/gizmoplane.frag"));
    shaders["skybox"] = Shader(asset("/shaders/skybox.vert"), asset("/shaders/skybox.frag"));

    // MaterialTexture container1 = MaterialTexture{asset("/textures/container.jpg"), GL_RGB};
    // MaterialTexture container2 = MaterialTexture{asset("/textures/container2.png"), GL_RGBA};
    // MaterialTexture container2_specular = MaterialTexture{asset("/textures/container2_specular.png"), GL_RGBA};
    // MaterialTexture awesomeface = MaterialTexture{asset("/textures/awesomeface.png"), GL_RGBA};
    // state->engine_data.textures = {container1, container2, container2_specular, awesomeface};

    Model light = Model(asset("/models/light/light.obj"));
    // Model backpack = Model(asset("/models/backpack/backpack.obj"));
    // Model barrel = Model(asset("/models/barrel/barrel.obj"));
    // Model room = Model(asset("/models/Isometric_Room_Blend/Isometric Room.obj"));
    Model planet = Model(asset("/models/planet/planet.obj"));
    Model testlevel = Model(asset("/models/testlevel/testlevel.obj"));
    state->rock = new Model(asset("/models/rock/rock.obj"));
    state->cylinder = new Model(asset("/models/cylinder/cylinder.obj"));



    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path pd = getExecutableDir();
    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Executable directory: " << pd << std::endl;
    std::cout << "Asset directory: " << ASSET_DIR << std::endl;
    std::cout << "Shader test: " << asset("/shaders/vert_lit.vert") << std::endl;

    
    const auto light_entity = state->m_registry.create();
    // Create light entity:
    state->m_registry.emplace<Transform>(light_entity, glm::vec3(1.2f, 1.0f, 2.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    state->m_registry.emplace<ModelWrapper>(light_entity, light);
    state->m_registry.emplace<RenderingData>(light_entity, &state->engine_data.shaders["light"]);
    state->m_registry.emplace<PointLight>(light_entity, 
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        1.0f, 0.09f, 0.032f
    );
    state->m_registry.emplace<LightTag>(light_entity);

    const auto light_entity2 = state->m_registry.create();
    state->m_registry.emplace<Transform>(light_entity2, glm::vec3(5.0f, 0.0f, 2.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    state->m_registry.emplace<ModelWrapper>(light_entity2, light);
    state->m_registry.emplace<RenderingData>(light_entity2, &state->engine_data.shaders["light"]);
    state->m_registry.emplace<SpotLight>(light_entity2, 
        glm::vec4(-5.0f, 0.0f, 2.0f, 1.0f),
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), 
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f))
    );
    state->m_registry.emplace<LightTag>(light_entity2);

    const auto dirlight = state->m_registry.create();
    state->m_registry.emplace<Transform>(dirlight, glm::vec3(0.0f, 5.0f, 0.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    state->m_registry.emplace<ModelWrapper>(dirlight, light);
    state->m_registry.emplace<RenderingData>(dirlight, &state->engine_data.shaders["light"]);
    state->m_registry.emplace<DirectionalLight>(dirlight, 
        glm::vec4(0.3f, -1.0f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), 
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), 
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    );
    state->m_registry.emplace<LightTag>(dirlight);


    const auto test_level_entity = state->m_registry.create();
    state->m_registry.emplace<Transform>(test_level_entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
    state->m_registry.emplace<ModelWrapper>(test_level_entity, testlevel);
    state->m_registry.emplace<RenderingData>(test_level_entity, &state->engine_data.shaders["model"]);




    // const auto planet_entity = state->m_registry.create();
    // state->m_registry.emplace<Transform>(planet_entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // state->m_registry.emplace<ModelWrapper>(planet_entity, planet);
    // state->m_registry.emplace<RenderingData>(planet_entity, &state->engine_data.shaders["model"]);

    // float positionRange[] = {-20.0f, 20.0f}; float scaleRange[] = {2.0f, 5.0f};
    // test_performance_entities(state->m_registry, 100, positionRange, scaleRange, &room, &state->engine_data.shaders["model"]);


    // Set Game Camera
    state->camera = Camera(glm::vec3(0.0f,0.0f,10.0f));

    // Assign events to systems.
    // m_dispatcher.sink<KeyDown>().connect<&MoveSystem::on_key_down>(m_move_system);
    // m_dispatcher.sink<KeyUp>().connect<&MoveSystem::on_key_up>(m_move_system);

    // Assign events to window.
   state->m_dispatcher.sink<KeyDown>().connect<&WindowManager::OnKeyDown>(windowManager);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    glEnable(GL_CULL_FACE); 
    // glCullFace(GL_FRONT); 
    glEnable(GL_MULTISAMPLE); 


    // Because its a retina display wtf?? Gets reset on size change
    windowManager->width *= windowManager->contentScale.x;
    windowManager->height *= windowManager->contentScale.y;
    glViewport(0, 0, windowManager->width, windowManager->height);

}

const int Game::Run(ImGuiContext *hostContext)
{
    
    // timing
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f;
    
    GLFWwindow *window = windowManager->GetWindow();

    ImGui::SetCurrentContext(hostContext);
    GUISystem::Initialise();

    // Setting Up Rendering Data

    Primitives::SetupQuadVAO(&state->quadVAO);
    Primitives::SetupQuadNormalsVAO(&state->quadNormalsVAO);
    Primitives::SetupRayVAO(&state->rayVAO);
    Primitives::SetupSkyboxVAO(&state->skyboxVAO);

    // Primitives::SetupTest();

    std::vector<std::string> faces { "px.png", "nx.png", "py.png", "ny.png", "pz.png", "nz.png" };
    state->skyboxTexture = TextureSystem::LoadCubemap(faces, asset("/textures/cubemaps/forest2/"), GL_RGBA);
    

    Graphics::SetupFramebuffer(&state->postProcessBuffer, glm::ivec2(windowManager->width, windowManager->height), &state->engine_data.shaders["postprocess"]);
    Graphics::SetupFramebuffer(&state->editorViewBuffer, glm::ivec2(windowManager->width, windowManager->height), &state->engine_data.shaders["editorview"]);

    // Uniform Buffer Object Stuff
    Primitives::SetupUniformBuffer(state->uboMatrices, 2 * sizeof(glm::mat4)+16, 0); // Matrices
    Primitives::SetupUniformBuffer(state->uboLights, 784, 1); // Lights

    Primitives::SetShaderUniformBuffer(state->engine_data.shaders["model"].ID, "Matrices", 0); // Matrices
    Primitives::SetShaderUniformBuffer(state->engine_data.shaders["model"].ID, "Lights", 1); // Lights

    Primitives::SetShaderUniformBuffer(state->engine_data.shaders["light"].ID, "Matrices", 0); // Matrices

    Primitives::SetShaderUniformBuffer(state->engine_data.shaders["rayquad"].ID, "Matrices", 0); // Matrices
    Primitives::SetShaderUniformBuffer(state->engine_data.shaders["gizmoplane"].ID, "Matrices", 0); // Matrices

    Primitives::SetShaderUniformBuffer(state->engine_data.shaders["skybox"].ID, "Lights", 1); // Lights


    // Primitives::SetShaderUniformBuffer(state->engine_data.shaders["instanced"].ID, "Matrices", 0); // Matrices
    // Primitives::SetShaderUniformBuffer(state->engine_data.shaders["instanced"].ID, "Lights", 1); // Lights

    // RenderSystem::BindVertexArray(state->m_registry, true); // Making sure to set all VBOs and VAOs to new values
    // TextureSystem::LoadTextures(state->engine_data.textures);


    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int reloaded = Events(deltaTime);

        // Update();
        Render();
        if (reloaded) {
            Shutdown();
            return 1;
        }
    }

    return 0;
}

bool pressedEscLastFrame = false;

int Game::Events(float deltaTime)
{
    GLFWwindow* window = windowManager->GetWindow();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!pressedEscLastFrame) {
            windowManager->ChangeMouseMode((state->useMouse == true) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            state->useMouse = !state->useMouse;
            pressedEscLastFrame = true;
        }
    } else if (pressedEscLastFrame) {
        pressedEscLastFrame = false;
        state->firstMouse = true; // Mouse gets reset when cursor mode changed
    }
    // glfwSetWindowShouldClose(window, true);
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //     windowManager.ChangeMouseMode(GLFW_CURSOR_DISABLED);
    //     firstMouse = true; // Mouse gets reset when cursor mode changed
    //     useMouse = true;
    // }

    // Allow dragging out of viewport only if click starts inside viewport.
    switch (glfwGetMouseButton(windowManager->GetWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
        case GLFW_PRESS:
            state->clickOnWindow.MouseDown(glm::vec2(state->lastX, state->lastY));
            Editor::lastClickInsideEditorWindow = Physics::PointInsideBBox2D( glm::vec2(state->lastX, state->lastY), Editor::viewRect);
            break;
        case GLFW_RELEASE:
            Editor::lastClickInsideEditorWindow = false;
            state->clickOnWindow.MouseUp();
            break;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        state->camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        state->camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        state->camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        state->camera.ProcessKeyboard(RIGHT, deltaTime);

#ifdef DEBUG
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        std::cout << "Hot reloading" << std::endl;
        return 1;
    }
#endif

    return 0;
}

void Game::Render()
{
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    auto start = std::chrono::high_resolution_clock::now();

    glViewport(0, 0, state->editorViewBuffer.size.x, state->editorViewBuffer.size.y); // Set Viewport to size of editor window

    // Render to post process buffer
    glBindFramebuffer(GL_FRAMEBUFFER, state->postProcessBuffer.fbo);
    glClearColor(state->clear_color.x, state->clear_color.y, state->clear_color.z, state->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    glEnable(GL_DEPTH_TEST);
    int focused = glfwGetWindowAttrib(windowManager->GetWindow(), GLFW_FOCUSED);
    if (focused) {
        if (Gizmos::globalGizmo.transform != nullptr) {
            glm::vec3 pointerRayDirection = RenderSystem::PointerToRay(glm::vec2(state->lastX, state->lastY));
            Gizmos::CalculateGizmoTransform(state->camera.Position, pointerRayDirection, Gizmos::globalGizmo);
            RenderSystem::Render(state->m_registry, state->fov, state->camera, state->editorViewBuffer.size);
            RenderSystem::RenderGizmos(Gizmos::globalGizmo, state->camera.Position, pointerRayDirection);
        } else {
            RenderSystem::Render(state->m_registry, state->fov, state->camera, state->editorViewBuffer.size); // Also check screen bounds
        }
        // RenderSystem::DrawLine(state->lastCameraPos+glm::vec3(0,-1,0), state->objectPos);
        Debug::LogErrors();
    }
    glDisable(GL_DEPTH_TEST);

    // Render to editor view buffer
    glBindFramebuffer(GL_FRAMEBUFFER, state->editorViewBuffer.fbo); // editor view buffer
    glClearColor(state->clear_color.x, state->clear_color.y, state->clear_color.z, state->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT); // we're not using the stencil buffer now
    Graphics::DrawFrame(&state->postProcessBuffer, state->quadVAO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    glClearColor(state->clear_color.x, state->clear_color.y, state->clear_color.z, state->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowManager->width, windowManager->height);

    // Graphics::DrawFrame(&state->postProcessBuffer, state->quadVAO);

    auto stop = std::chrono::high_resolution_clock::now();

    time_map["8 Entities Render"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;


    start = std::chrono::high_resolution_clock::now();

    GUISystem::DrawSideBar(state->m_registry, state, &state->engine_data, &reload_shaders, &Init);
    stop = std::chrono::high_resolution_clock::now();
    time_map["1 ImGui Fill"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;

    // RenderSystem::Render(*windowManager, state->m_registry, state->fov, state->camera);

    start = std::chrono::high_resolution_clock::now();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    stop = std::chrono::high_resolution_clock::now();

    time_map["9 ImGui Render"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    // start = std::chrono::high_resolution_clock::now();
    glfwSwapBuffers(windowManager->GetWindow());
    glfwPollEvents();
    stop = std::chrono::high_resolution_clock::now();
    // time_map["0 Swap Buffers Poll Events"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;
}

void Game::Shutdown() {
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    windowManager->SetSize(width, height);
    // Graphics::ChangeFramebufferSize(&state->postProcessBuffer, glm::ivec2(width, height));
    // Graphics::ChangeFramebufferSize(&state->editorViewBuffer, glm::ivec2(width, height));
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // std::cout << "mouse moved " << xposIn << " " << yposIn << std::endl;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (state->useMouse) {
        if (state->firstMouse)
        {
            state->lastX = xpos;
            state->lastY = ypos;
            state->firstMouse = false;
        }

        float xoffset = xpos - state->lastX;
        float yoffset = state->lastY - ypos; // reversed since y-coordinates go from bottom to top

        state->camera.ProcessMouseMovement(xoffset, yoffset);
    }
    state->lastX = xpos;
    state->lastY = ypos;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    state->camera.ProcessMouseScroll(yoffset);
}

void reload_shaders() {
    std::cout << "Shaders Reloaded" << std::endl;
    for (auto& [shaderName, shader] : state->engine_data.shaders) {
        shader.load();

        Primitives::SetShaderUniformBuffer(state->engine_data.shaders["model"].ID, "Matrices", 0); // Matrices
        Primitives::SetShaderUniformBuffer(state->engine_data.shaders["model"].ID, "Lights", 1); // Lights

        Primitives::SetShaderUniformBuffer(state->engine_data.shaders["light"].ID, "Matrices", 0); // Matrices

        Primitives::SetShaderUniformBuffer(state->engine_data.shaders["skybox"].ID, "Lights", 1); // Lights

    };
}


void test_performance_entities(entt::registry &m_registry, int numObjects, float positionRange[2], float scaleRange[2], Model *model, Shader *shader) {
    // Test performance with entities:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(positionRange[0], positionRange[1]);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(scaleRange[0], scaleRange[1]);
    std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    for (int i = 0; i < numObjects; i++)
    {
        glm::vec3 position = glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator));
        glm::mat4 rotation = glm::mat4_cast(glm::quat(glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator))));
        glm::vec3 scale = glm::vec3(randScale(generator), randScale(generator), randScale(generator));
        const auto model_test_entity = m_registry.create();
        m_registry.emplace<Transform>(model_test_entity, position, rotation, scale);
        m_registry.emplace<ModelWrapper>(model_test_entity, *model);
        m_registry.emplace<RenderingData>(model_test_entity, shader);
    }
}