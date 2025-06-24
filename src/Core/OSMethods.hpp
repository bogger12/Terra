#pragma once

#include <filesystem>
#ifdef _WIN32
#include <libloaderapi.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif
#include <string>

#ifndef ASSET_DIR
#define ASSET_DIR "assets"
#endif

inline std::filesystem::path getExecutableDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
#elif __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        // buffer too small
        return "";
    }
#elif __linux__
    char path[1024];
    ssize_t count = readlink("/proc/self/exe", path, sizeof(path));
    if (count == -1) return "";
    path[count] = '\0';
#endif
    return std::filesystem::path(path).parent_path();
}

inline std::string asset(const char* assetpath) {
    return (getExecutableDir().string() + "/" + ASSET_DIR + assetpath);
}