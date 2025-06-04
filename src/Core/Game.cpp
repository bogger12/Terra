#include <random>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Game.hpp"
#include "../Components/All.hpp"
#include "../Events/KeyDown.hpp"
#include <camera.h>
#include "../Systems/RenderSystem.hpp"
#include "../Systems/TextureSystem.hpp"
#include "../Systems/GUISystem.hpp"
#include "Globals.hpp"

#ifndef ASSET_DIR
#define ASSET_DIR "../assets"
#endif

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void reload_shaders();
void test_performance_entities(entt::registry &m_registry, std::vector<float> &cube_vertices, int numObjects, float positionRange[2], float scaleRange[2]);

WindowManager windowManager = WindowManager();


float fov = 45.0f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool useMouse = true;

Camera camera;

GlobalState global_state;
EngineData engine_data;


Game::Game(std::string windowName, const int w, const int h)
{
    windowManager.Create(w, h, windowName, framebuffer_size_callback, mouse_callback, scroll_callback);

    // Here, we are creating the entities using EnTT and attaching the relevant components and tags.
    // We can invoke the constructor of the component or tag in the assign() and attach() methods of the registry.

    Shader default_shader = Shader(ASSET_DIR "/shaders/vert_lit.glsl", ASSET_DIR "/shaders/frag_lit.glsl");
    Shader light_source_shader = Shader(ASSET_DIR "/shaders/vert_light.glsl", ASSET_DIR "/shaders/frag_light.glsl");
    engine_data.shaders[0] = default_shader;
    engine_data.shaders[1] = light_source_shader;
    Texture container1 = Texture{ASSET_DIR "/textures/container.jpg", GL_RGB};
    Texture container2 = Texture{ASSET_DIR "/textures/container2.png", GL_RGBA};
    Texture awesomeface = Texture{ASSET_DIR "/textures/awesomeface.png", GL_RGBA};
    engine_data.textures = {container1, container2, awesomeface};

    const auto cube_entity = m_registry.create();
    // const auto cube_entity2 = m_registry.create();
    const auto light_entity = m_registry.create();

    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Asset directory: " << ASSET_DIR << std::endl;
    // unsigned int textureID = LoadTextureFromPath(ASSET_DIR "/textures/awesomeface.png", width, height, nrChannels, GL_RGBA);

    std::vector<float> cube_vertices = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    float positionRange[] = {-50.0f, 50.0f}; float scaleRange[] = {1.0f, 2.0f};
    test_performance_entities(m_registry, cube_vertices, 2000, positionRange, scaleRange);

    // Assign component data to entities.
    m_registry.emplace<Transform>(cube_entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // m_registry.emplace<Texture>(cube_entity, ASSET_DIR "/textures/awesomeface.png", GL_RGBA);
    m_registry.emplace<ModelData>(cube_entity, cube_vertices);
    m_registry.emplace<RenderingData>(cube_entity, &engine_data.shaders[0], 
        Material{
            glm::vec3(1.0f, 0.5f, 0.31f), // Albedo
            &engine_data.textures[1], // Container 2
            glm::vec3(1.0f, 0.5f, 0.31f), // Diffuse
            glm::vec3(0.5f, 0.5f, 0.5f), // Specular
            32.0f // Shininess
        }
    );


    // Create light entity:
    m_registry.emplace<Transform>(light_entity, glm::vec3(1.2f, 1.0f, 2.0f), glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
    m_registry.emplace<ModelData>(light_entity, cube_vertices);
    m_registry.emplace<RenderingData>(light_entity, &engine_data.shaders[1]);
    m_registry.emplace<Light>(light_entity, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

    // Set Game Camera
    camera = Camera(glm::vec3(0.0f,0.0f,3.0f));

    // Assign events to systems.
    // m_dispatcher.sink<KeyDown>().connect<&MoveSystem::on_key_down>(m_move_system);
    // m_dispatcher.sink<KeyUp>().connect<&MoveSystem::on_key_up>(m_move_system);

    // Assign events to window.
    m_dispatcher.sink<KeyDown>().connect<&WindowManager::OnKeyDown>(windowManager);

}

const int Game::Run()
{
    
    // timing
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f;

    RenderSystem::BindVertexArray(m_registry);
    TextureSystem::LoadTextures(engine_data.textures);

    GLFWwindow *window = windowManager.GetWindow();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Events(deltaTime);

        // Update();

        Render();
    }

    glfwTerminate();
    return 0;
}

bool pressedEscLastFrame = false;

void Game::Events(float deltaTime)
{
    GLFWwindow* window = windowManager.GetWindow();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!pressedEscLastFrame) {
            windowManager.ChangeMouseMode((useMouse == true) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
            useMouse = !useMouse;
            pressedEscLastFrame = true;
        }
    } else if (pressedEscLastFrame) {
        pressedEscLastFrame = false;
        firstMouse = true; // Mouse gets reset when cursor mode changed
    }
    // glfwSetWindowShouldClose(window, true);
    // if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //     windowManager.ChangeMouseMode(GLFW_CURSOR_DISABLED);
    //     firstMouse = true; // Mouse gets reset when cursor mode changed
    //     useMouse = true;
    // }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    // {
    //     std::cout << "Shaders Reloaded" << std::endl;
    //     default_shader = Shader(ASSET_DIR "/shaders/vert_lit.glsl", ASSET_DIR "/shaders/frag_lit.glsl");
    //     light_source_shader = Shader(ASSET_DIR "/shaders/vert_light.glsl", ASSET_DIR "/shaders/frag_light.glsl");
    // }
}

void Game::Render()
{
    // render
    // ------
    glClearColor(global_state.clear_color.x * global_state.clear_color.w, global_state.clear_color.y * global_state.clear_color.w, global_state.clear_color.z * global_state.clear_color.w, global_state.clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto start = std::chrono::high_resolution_clock::now();
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (global_state.show_demo_window) ImGui::ShowDemoWindow(); // Show demo window! :)

    GUISystem::DrawSideBar(m_registry, &global_state, &engine_data, windowManager, &reload_shaders);
    auto stop = std::chrono::high_resolution_clock::now();
    global_state.time_map["1 ImGui Fill"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;

    start = std::chrono::high_resolution_clock::now();
    RenderSystem::Render(windowManager, m_registry, fov, camera);
    stop = std::chrono::high_resolution_clock::now();

    global_state.time_map["8 Entities Render"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;
    
    start = std::chrono::high_resolution_clock::now();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    stop = std::chrono::high_resolution_clock::now();

    global_state.time_map["9 ImGui Render"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    // start = std::chrono::high_resolution_clock::now();
    glfwSwapBuffers(windowManager.GetWindow());
    glfwPollEvents();
    stop = std::chrono::high_resolution_clock::now();
    // global_state.time_map["0 Swap Buffers Poll Events"] = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()/1000.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    windowManager.SetSize(width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // std::cout << "mouse moved " << xposIn << " " << yposIn << std::endl;
    if (!useMouse) return;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void reload_shaders() {
    std::cout << "Shaders Reloaded" << std::endl;
    for (Shader shader : engine_data.shaders) {
        shader.load();
    };
}


void test_performance_entities(entt::registry &m_registry, std::vector<float> &cube_vertices, int numObjects, float positionRange[2], float scaleRange[2]) {
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
        const auto cube_entity = m_registry.create();
        m_registry.emplace<Transform>(cube_entity, position, rotation, scale);
        m_registry.emplace<ModelData>(cube_entity, cube_vertices);
        m_registry.emplace<RenderingData>(cube_entity, &engine_data.shaders[0], 
            Material{glm::vec3(1.0f, 0.5f, 0.31f), &engine_data.textures[1], glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f}
        );
    }
}