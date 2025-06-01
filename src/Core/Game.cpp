#include <random>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Game.hpp"
#include "../Components/All.hpp"
#include "../Events/KeyDown.hpp"
#include <camera.h>
#include "../Systems/RenderSystem.hpp"
#include "../Systems/Texture.hpp"

#ifndef ASSET_DIR
#define ASSET_DIR "../assets"
#endif

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

WindowManager windowManager = WindowManager();


float fov = 45.0f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool useMouse = true;

Camera camera;

Shader default_shader;
Shader light_source_shader;

Game::Game(std::string windowName, const int w, const int h)
    : renderSystem{}
{
    windowManager.Create(w, h, windowName, framebuffer_size_callback, mouse_callback, scroll_callback);

    // Here, we are creating the entities using EnTT and attaching the relevant components and tags.
    // We can invoke the constructor of the component or tag in the assign() and attach() methods of the registry.

    default_shader = Shader(ASSET_DIR "/shaders/vert_lit.glsl", ASSET_DIR "/shaders/frag_lit.glsl");
    light_source_shader = Shader(ASSET_DIR "/shaders/vert_light.glsl", ASSET_DIR "/shaders/frag_light.glsl");

    const auto cube_entity = m_registry.create();
    // const auto cube_entity2 = m_registry.create();
    const auto light_entity = m_registry.create();

    // Load texture:
    int width, height, nrChannels;
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;
    std::cout << "Asset directory: " << ASSET_DIR << std::endl;
    unsigned int textureID = LoadTextureFromPath(ASSET_DIR "/textures/awesomeface.png", width, height, nrChannels, GL_RGBA);

    std::vector<float> cube_vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };



    // Assign component data to entities.
    m_registry.emplace<Transform>(cube_entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    m_registry.emplace<Texture>(cube_entity, textureID);
    m_registry.emplace<ModelData>(cube_entity, cube_vertices, &default_shader);

    // m_registry.emplace<Transform>(cube_entity2, glm::vec3(0.0f, 2.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.5f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    // m_registry.emplace<Texture>(cube_entity2, textureID);
    // m_registry.emplace<ModelData>(cube_entity2, cube_vertices, &default_shader);


    // Create light entity:
    m_registry.emplace<Transform>(light_entity, glm::vec3(1.2f, 1.0f, 2.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(0.1f, 0.1f, 0.1f));
    // m_registry.emplace<Texture>(light_entity, textureID);
    m_registry.emplace<ModelData>(light_entity, cube_vertices, &light_source_shader);
    m_registry.emplace<Light>(light_entity);

    // Set Game Camera
    camera = Camera();

    // Assign events to systems.
    // m_dispatcher.sink<KeyDown>().connect<&MoveSystem::on_key_down>(m_move_system);
    // m_dispatcher.sink<KeyUp>().connect<&MoveSystem::on_key_up>(m_move_system);

    // Assign events to window.
    m_dispatcher.sink<KeyDown>().connect<&WindowManager::OnKeyDown>(windowManager);

    // Set up collideables
    // m_collideables.ai = ai_paddle;
    // m_collideables.player = player_paddle;
    // m_collideables.ball = ball;
    // m_collideables.registery = &m_registry;
}

const int Game::Run()
{
    
    // timing
    float deltaTime = 0.0f; // time between current frame and last frame
    float lastFrame = 0.0f;

    renderSystem.BindVertexArray(m_registry);

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

void Game::Events(float deltaTime)
{
    GLFWwindow* window = windowManager.GetWindow();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        windowManager.ChangeMouseMode(GLFW_CURSOR_NORMAL);
        useMouse = false;
    }
    // glfwSetWindowShouldClose(window, true);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        windowManager.ChangeMouseMode(GLFW_CURSOR_DISABLED);
        firstMouse = true; // Mouse gets reset when cursor mode changed
        useMouse = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        std::cout << "Shaders Reloaded" << std::endl;
        default_shader = Shader(ASSET_DIR "/shaders/vert_lit.glsl", ASSET_DIR "/shaders/frag_lit.glsl");
        light_source_shader = Shader(ASSET_DIR "/shaders/vert_light.glsl", ASSET_DIR "/shaders/frag_light.glsl");
    }
}

void Game::Render()
{
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(); // Show demo window! :)

    renderSystem.Render(windowManager, m_registry, fov, camera);

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(windowManager.GetWindow());
    glfwPollEvents();
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
