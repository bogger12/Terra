#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "../Core/GameState.hpp"



class Debug {
    public:
        static void Log(std::string logString) {
            state->logStream << logString << std::endl;
        }

        static std::string ReadLog() {
            return state->logStream.str();
        }
        static std::ostringstream& LogStream() {
            return state->logStream;
        }

        static void LogErrors() {
            GLenum err;
            while((err = glGetError()) != GL_NO_ERROR)
            {
                state->logStream << "GL ERROR: " << err << std::endl;
            }
        }
};