#pragma once

#include <platformGL.h>
#include <glm/glm.hpp>

#include <string>

class WindowManager {
    public:
        int width, height;
        glm::vec2 contentScale;
        GLFWwindow* Create(int width, int height, std::string windowName);
        static void InitialiseGlad();
        void SetCallbacks(GLFWframebuffersizefun framebuffer_size_callback, 
            GLFWcursorposfun mouse_callback, 
            GLFWscrollfun scroll_callback); 
        void InitialiseGUI();
        void ShutdownGUI();
        GLFWwindow* GetWindow() { return window; };
        void SetSize(int newWidth, int newHeight) { width = newWidth; height = newHeight; };
        void Close() noexcept;
        void OnKeyDown();
        void ChangeMouseMode(int value);
        static void ChangeVSync(bool);

    private:
        GLFWwindow* window;
};

extern WindowManager *windowManager;