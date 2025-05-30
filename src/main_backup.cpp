

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <stb_image.h>

// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// #include <shader.h>

// #include <iostream>
// #include "Engine/GameObject.h"
// #include "Engine/Transform.h"
// #include "Engine/Texture.h"
// #include "Engine/ModelRenderer.h"
// #include <camera.h>

// void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void mouse_callback(GLFWwindow *window, double xpos, double ypos);
// void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
// void processInput(GLFWwindow *window);

// // settings
// const unsigned int SCR_WIDTH = 800;
// const unsigned int SCR_HEIGHT = 600;

// // Camera
// Camera camera;
// float fov = 45.0f;
// bool firstMouse = true;
// float lastX = 800.0f / 2.0;
// float lastY = 600.0 / 2.0;

// // timing
// float deltaTime = 0.0f; // time between current frame and last frame
// float lastFrame = 0.0f;

// int main()
// {

//     // START HERE:
//     camera = Camera();

//     GameObject myObject = GameObject(glm::vec3(0, 0, 0));
//     myObject.modelRenderer.texture.internalFormat = GL_RGBA;
//     myObject.modelRenderer.texture.LoadFromPath("assets/textures/awesomeface.png");

//     std::cout << myObject.modelRenderer.texture.height << std::endl;

//     // set up vertex data (and buffer(s)) and configure vertex attributes
//     // ------------------------------------------------------------------
//     std::vector<float> vertices = {
//         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
//         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
//         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

//         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
//         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
//         -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
//         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

//         -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//         -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//         -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

//         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//         0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

//         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
//         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
//         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
//         -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
//         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

//         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
//         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
//         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
//         -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
//         -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

//     // myObject.modelRenderer.vertices = vertices;

//     // // world space positions of our cubes
//     glm::vec3 cubePositions[] = {
//         glm::vec3(0.0f, 0.0f, 0.0f),
//         glm::vec3(2.0f, 5.0f, -15.0f),
//         glm::vec3(-1.5f, -2.2f, -2.5f),
//         glm::vec3(-3.8f, -2.0f, -12.3f),
//         glm::vec3(2.4f, -0.4f, -3.5f),
//         glm::vec3(-1.7f, 3.0f, -7.5f),
//         glm::vec3(1.3f, -2.0f, -2.5f),
//         glm::vec3(1.5f, 2.0f, -2.5f),
//         glm::vec3(1.5f, 0.2f, -1.5f),
//         glm::vec3(-1.3f, 1.0f, -1.5f)};

//     myObject.modelRenderer.BindVertexArray();

//     myObject.modelRenderer.InitialiseShader();

//     // render loop
//     // -----------
//     while (!glfwWindowShouldClose(window))
//     {
//         // per-frame time logic
//         // --------------------
//         float currentFrame = static_cast<float>(glfwGetTime());
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;

//         // input
//         // -----
//         processInput(window);

//         // render
//         // ------
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         // bind textures on corresponding texture units
//         myObject.modelRenderer.texture.BindTexture();

//         // activate shader
//         myObject.modelRenderer.modelShader.use();

//         // pass projection matrix to shader (note that in this case it could change every frame)
//         glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//         myObject.modelRenderer.modelShader.setMat4("projection", projection);

//         // camera/view transformation
//         glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
//         myObject.modelRenderer.modelShader.setMat4("view", view);

//         // render boxes
//         // glBindVertexArray(VAO);
//         for (unsigned int i = 0; i < 10; i++)
//         {
//             // calculate the model matrix for each object and pass it to shader before drawing
//             glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//             model = glm::translate(model, cubePositions[i]);
//             float angle = 20.0f * i;
//             model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//             myObject.modelRenderer.modelShader.setMat4("model", model);

//             // glDrawArrays(GL_TRIANGLES, 0, 36);
//             myObject.modelRenderer.Render();
//         }

//         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//         // -------------------------------------------------------------------------------
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     // optional: de-allocate all resources once they've outlived their purpose:
//     // ------------------------------------------------------------------------
//     // glDeleteVertexArrays(1, &VAO);
//     // glDeleteBuffers(1, &VBO);

//     // glfw: terminate, clearing all previously allocated GLFW resources.
//     // ------------------------------------------------------------------
//     glfwTerminate();
//     return 0;
// }

// // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// // ---------------------------------------------------------------------------------------------------------
// void processInput(GLFWwindow *window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);

//     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//         camera.ProcessKeyboard(FORWARD, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//         camera.ProcessKeyboard(BACKWARD, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//         camera.ProcessKeyboard(LEFT, deltaTime);
//     if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//         camera.ProcessKeyboard(RIGHT, deltaTime);
// }

// // glfw: whenever the window size changed (by OS or user resize) this callback function executes
// // ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow *window, int width, int height)
// {
//     // make sure the viewport matches the new window dimensions; note that width and
//     // height will be significantly larger than specified on retina displays.
//     glViewport(0, 0, width, height);
// }

// // glfw: whenever the mouse moves, this callback is called
// // -------------------------------------------------------
// void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
// {
//     float xpos = static_cast<float>(xposIn);
//     float ypos = static_cast<float>(yposIn);

//     if (firstMouse)
//     {
//         lastX = xpos;
//         lastY = ypos;
//         firstMouse = false;
//     }

//     float xoffset = xpos - lastX;
//     float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//     lastX = xpos;
//     lastY = ypos;

//     camera.ProcessMouseMovement(xoffset, yoffset);
// }

// // glfw: whenever the mouse scroll wheel scrolls, this callback is called
// // ----------------------------------------------------------------------
// void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(yoffset);
// }
