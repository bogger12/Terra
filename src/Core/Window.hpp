#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
    public:
        Window();
        ~Window();
        int width, height;
        void Create(int w, int h, std::string windowName,
            GLFWframebuffersizefun framebuffer_size_callback,
            GLFWcursorposfun mouse_callback,
            GLFWscrollfun scroll_callback);
        GLFWwindow* GetWindow() { return window; };
        void Close() noexcept;
        void OnKeyDown();
        void ChangeMouseMode(int value);

    private:
        GLFWwindow* window;
};