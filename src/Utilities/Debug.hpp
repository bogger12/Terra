#pragma once
#include <string>
#include <iostream>
#include <sstream>


class Debug {
    public:
        static std::ostringstream logStream;
        static void Log(std::string logString) {
            logStream << logString << "\n";
        }

        static std::string ReadLog() {
            return logStream.str();
        }
};