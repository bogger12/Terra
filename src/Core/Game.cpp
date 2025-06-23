#include <random>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include "Game.hpp"
#include "../Components/All.hpp"
#include "../Events/KeyDown.hpp"
#include <camera.h>
#include "../Systems/RenderSystem.hpp"
#include "../Systems/TextureSystem.hpp"
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

extern "C" void CreateGame(WindowManager *gameWindowManager, GameState *gameState, bool isCreated) {
    state = gameState;
    windowManager = gameWindowManager;
    WindowManager::InitialiseGlad();
    windowManager->SetCallbacks(framebuffer_size_callback, mouse_callback, scroll_callback);
    // glfwMakeContextCurrent(gameWindowManager->GetWindow());
    if (!isCreated) Game::Init(gameWindowManager, gameState);
}

extern "C" int RunGame(ImGuiContext *hostContext) {
    glfwMakeContextCurrent(windowManager->GetWindow()); // `window` must be a valid GLFWwindow*
    WindowManager::InitialiseGlad(); // Initialise GLAD function pointers again as they get removed during hot reload

    return Game::Run(hostContext);
}

// Game Code

void Game::Init(WindowManager *gameWindowManager, GameState *gameState)
{

    // Setting Shaders
    Shader default_shader = Shader(asset("/shaders/vert_lit.vert"), asset("/shaders/frag_lit.frag"));
    Shader light_source_shader = Shader(asset("/shaders/light.vert"), asset("/shaders/light.frag"));
    Shader model_shader1 = Shader(asset("/shaders/model_lit.vert"), asset("/shaders/model_lit.frag"));
    Shader model_shader2 = Shader(asset("/shaders/model_lit2.vert"), asset("/shaders/model_lit2.frag"));
    state->engine_data.shaders = {default_shader, light_source_shader, model_shader1, model_shader2};
    MaterialTexture container1 = MaterialTexture{asset("/textures/container.jpg"), GL_RGB};
    MaterialTexture container2 = MaterialTexture{asset("/textures/container2.png"), GL_RGBA};
    MaterialTexture container2_specular = MaterialTexture{asset("/textures/container2_specular.png"), GL_RGBA};
    MaterialTexture awesomeface = MaterialTexture{asset("/textures/awesomeface.png"), GL_RGBA};
    state->engine_data.textures = {container1, container2, container2_specular, awesomeface};

    // Model backpack = Model(asset("/models/backpack/backpack.obj"));
    Model barrel = Model(asset("/models/barrel/barrel.obj"));
    Model light = Model(asset("/models/light/light.obj"));
    // Model heart = Model(asset("/models/Human_Heart/Human_Heart.obj"));
    Model nswitch = Model(asset("/models/switch/Mario_game_world_Scene.obj"));



    std::filesystem::path cwd = std::filesystem::current_path();
    std::filesystem::path pd = getExecutableDir();
    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Executable directory: " << pd << std::endl;
    std::cout << "Asset directory: " << ASSET_DIR << std::endl;
    std::cout << "Shader test: " << asset("/shaders/vert_lit.vert") << std::endl;

    
    // float positionRange[] = {-20.0f, 20.0f}; float scaleRange[] = {2.0f, 5.0f};
    // test_performance_entities(state->m_registry, 200, positionRange, scaleRange, &barrel, &state->engine_data.shaders[2]);

    const auto light_entity = state->m_registry.create();
    // Create light entity:
    state->m_registry.emplace<Transform>(light_entity, glm::vec3(1.2f, 1.0f, 2.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    state->m_registry.emplace<ModelWrapper>(light_entity, light);
    state->m_registry.emplace<RenderingData>(light_entity, &state->engine_data.shaders[1]);
    state->m_registry.emplace<PointLight>(light_entity, 
        glm::vec3(0.2f, 0.2f, 0.2f), 
        glm::vec3(0.5f, 0.5f, 0.5f), 
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f, 0.09f, 0.032f
    );
    state->m_registry.emplace<LightTag>(light_entity);

    const auto light_entity2 = state->m_registry.create();
    state->m_registry.emplace<Transform>(light_entity2, glm::vec3(1.2f, -1.0f, 2.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    state->m_registry.emplace<ModelWrapper>(light_entity2, light);
    state->m_registry.emplace<RenderingData>(light_entity2, &state->engine_data.shaders[1]);
    state->m_registry.emplace<PointLight>(light_entity2, 
        glm::vec3(0.2f, 0.2f, 0.2f), 
        glm::vec3(0.5f, 0.5f, 0.5f), 
        glm::vec3(1.0f, 1.0f, 1.0f),
        1.0f, 0.09f, 0.032f
    );
    state->m_registry.emplace<LightTag>(light_entity2);

    const auto light_entity3 = state->m_registry.create();
    state->m_registry.emplace<Transform>(light_entity3, glm::vec3(5.0f, 0.0f, 2.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    state->m_registry.emplace<ModelWrapper>(light_entity3, light);
    state->m_registry.emplace<RenderingData>(light_entity3, &state->engine_data.shaders[1]);
    state->m_registry.emplace<SpotLight>(light_entity3, 
        glm::vec3(0.2f, 0.2f, 0.2f), 
        glm::vec3(0.5f, 0.5f, 0.5f), 
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(-5.0f, 0.0f, 2.0f),
        12.5f, 17.5f
    );
    state->m_registry.emplace<LightTag>(light_entity3);

    // const auto model_test_entity = state->m_registry.create();
    // state->m_registry.emplace<Transform>(model_test_entity, glm::vec3(0.0f, 1.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // state->m_registry.emplace<ModelWrapper>(model_test_entity, backpack);
    // state->m_registry.emplace<RenderingData>(model_test_entity, &state->engine_data.shaders[2]);

    const auto model_test_entity2 = state->m_registry.create();
    state->m_registry.emplace<Transform>(model_test_entity2, glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    state->m_registry.emplace<ModelWrapper>(model_test_entity2, barrel);
    state->m_registry.emplace<RenderingData>(model_test_entity2, &state->engine_data.shaders[2]);
    
    const auto model_test_entity3 = state->m_registry.create();
    state->m_registry.emplace<Transform>(model_test_entity3, glm::vec3(0.5f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    state->m_registry.emplace<ModelWrapper>(model_test_entity3, barrel);
    state->m_registry.emplace<RenderingData>(model_test_entity3, &state->engine_data.shaders[2]);
    
    // const auto model_test_entity4 = state->m_registry.create();
    // state->m_registry.emplace<Transform>(model_test_entity4, glm::vec3(-1.5f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // state->m_registry.emplace<ModelWrapper>(model_test_entity4, heart);
    // state->m_registry.emplace<RenderingData>(model_test_entity4, &state->engine_data.shaders[2]);

    const auto model_test_entity5 = state->m_registry.create();
    state->m_registry.emplace<Transform>(model_test_entity5, glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    state->m_registry.emplace<ModelWrapper>(model_test_entity5, nswitch);
    state->m_registry.emplace<RenderingData>(model_test_entity5, &state->engine_data.shaders[2]);




    // Set Game Camera
    state->camera = Camera(glm::vec3(0.0f,0.0f,3.0f));

    // Assign events to systems.
    // m_dispatcher.sink<KeyDown>().connect<&MoveSystem::on_key_down>(m_move_system);
    // m_dispatcher.sink<KeyUp>().connect<&MoveSystem::on_key_up>(m_move_system);

    // Assign events to window.
   state->m_dispatcher.sink<KeyDown>().connect<&WindowManager::OnKeyDown>(windowManager);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

}

const int Game::Run(ImGuiContext *hostContext)
{
    
    // timing
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f;
    
    GLFWwindow *window = windowManager->GetWindow();

    ImGui::SetCurrentContext(hostContext);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        state->camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        state->camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        state->camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        state->camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        std::cout << "Hot reloading" << std::endl;
        return 1;
    }

    return 0;
}

void Game::Render()
{
    // render
    // ------
    glClearColor(state->clear_color.x * state->clear_color.w, state->clear_color.y * state->clear_color.w, state->clear_color.z * state->clear_color.w, state->clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto start = std::chrono::high_resolution_clock::now();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (state->show_demo_window) ImGui::ShowDemoWindow(); // Show demo window! :)

    GUISystem::DrawSideBar(state->m_registry, state, &state->engine_data, *windowManager, &reload_shaders);
    auto stop = std::chrono::high_resolution_clock::now();
    time_map["1 ImGui Fill"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;

    start = std::chrono::high_resolution_clock::now();
    RenderSystem::Render(*windowManager, state->m_registry, state->fov, state->camera);
    stop = std::chrono::high_resolution_clock::now();

    time_map["8 Entities Render"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;
    
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
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // std::cout << "mouse moved " << xposIn << " " << yposIn << std::endl;
    if (!state->useMouse) return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (state->firstMouse)
    {
        state->lastX = xpos;
        state->lastY = ypos;
        state->firstMouse = false;
    }

    float xoffset = xpos - state->lastX;
    float yoffset = state->lastY - ypos; // reversed since y-coordinates go from bottom to top
    state->lastX = xpos;
    state->lastY = ypos;

    state->camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    state->camera.ProcessMouseScroll(yoffset);
}

void reload_shaders() {
    std::cout << "Shaders Reloaded" << std::endl;
    for (Shader &shader : state->engine_data.shaders) {
        shader.load();
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