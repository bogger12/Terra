#include <random>

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

float fov = 45.0f;
bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

Camera camera;

Game::Game(std::string windowName, const int w, const int h)
    : window{}, renderSystem{}
{
    window.Create(w, h, windowName, framebuffer_size_callback, mouse_callback, scroll_callback);

    // Here, we are creating the entities using EnTT and attaching the relevant components and tags.
    // We can invoke the constructor of the component or tag in the assign() and attach() methods of the registry.

    const auto cube_entity = m_registry.create();
    const auto cube_entity2 = m_registry.create();

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-20.0f, 20.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    // Load texture:
    int width, height, nrChannels;
    std::filesystem::path cwd = std::filesystem::current_path();
    std::cout << "Current working directory: " << cwd << std::endl;
    unsigned int textureID = LoadTextureFromPath(ASSET_DIR "/textures/awesomeface.png", width, height, nrChannels, GL_RGBA);

    std::vector<float> cube_vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    // Assign component data to entities.
    m_registry.emplace<Transform>(cube_entity, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.0f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    m_registry.emplace<Texture>(cube_entity, textureID);
    m_registry.emplace<ModelData>(cube_entity, cube_vertices);

    m_registry.emplace<Transform>(cube_entity2, glm::vec3(0.0f, 2.0f, 0.0f), glm::quat(glm::vec3(0.0f, 0.5f, 0.0f)), glm::vec3(1.0f, 1.0f, 1.0f));
    m_registry.emplace<Texture>(cube_entity2, textureID);
    m_registry.emplace<ModelData>(cube_entity2, cube_vertices);

    for (int i = 0; i < 500; i++)
    {
        glm::vec3 position = glm::vec3(randPosition(generator), randPosition(generator), randPosition(generator));
        glm::vec3 rotation = glm::vec3(randRotation(generator), randRotation(generator), randRotation(generator));
        glm::vec3 scale = glm::vec3(randScale(generator), randScale(generator), randScale(generator));
        const auto cube_entity = m_registry.create();
        m_registry.emplace<Transform>(cube_entity, position, rotation, scale);
        m_registry.emplace<Texture>(cube_entity, textureID);
        m_registry.emplace<ModelData>(cube_entity, cube_vertices);
    }

    // Set Game Camera
    camera = Camera();

    // Assign events to systems.
    // m_dispatcher.sink<KeyDown>().connect<&MoveSystem::on_key_down>(m_move_system);
    // m_dispatcher.sink<KeyUp>().connect<&MoveSystem::on_key_up>(m_move_system);

    // Assign events to window.
    m_dispatcher.sink<KeyDown>().connect<&Window::OnKeyDown>(window);

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

    GLFWwindow *window = this->window.GetWindow();
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
    GLFWwindow* window = this->window.GetWindow();
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void Game::Render()
{
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    renderSystem.Render(window, m_registry, fov, camera);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window.GetWindow());
    glfwPollEvents();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    std::cout << "mouse moved " << xposIn << " " << yposIn << std::endl;
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
