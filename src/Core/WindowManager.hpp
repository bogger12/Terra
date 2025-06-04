#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class WindowManager {
    public:
        WindowManager();
        ~WindowManager();
        int width, height;
        void Create(int width, int height, std::string windowName,
            GLFWframebuffersizefun framebuffer_size_callback,
            GLFWcursorposfun mouse_callback,
            GLFWscrollfun scroll_callback);
        GLFWwindow* GetWindow() { return window; };
        void SetSize(int width, int height) { this->width = width; this->height = height; };
        void Close() noexcept;
        void OnKeyDown();
        void ChangeMouseMode(int value);
        void ChangeVSync(bool);

    private:
        GLFWwindow* window;
};