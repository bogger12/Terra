
cmake_minimum_required(VERSION 3.12)
project(Terra VERSION 1.0
    DESCRIPTION "OpenGL engine project"
    LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_BUILD_TYPE Debug)


find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)

message(STATUS "GLFW3_LIBRARY: ${GLFW3_LIBRARY}")
message(STATUS "GLFW3_INCLUDE_DIR: ${GLFW3_INCLUDE_DIR}")


# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/libraries)

# Change assets dir to be defined as local one
add_compile_definitions(ASSET_DIR="assets")

# Assign all sources to GAME_SOURCES list
file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")

add_library(TerraGame SHARED src/glad.c ${GAME_SOURCES})

set_target_properties(TerraGame PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TerraGame PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(TerraGame PROPERTIES PUBLIC_HEADER src/Core/Game.hpp)
target_include_directories(TerraGame PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_include_directories(TerraGame PRIVATE ${PROJECT_SOURCE_DIR}/src)
install(TARGETS TerraGame
    LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
    PUBLIC_HEADER DESTINATION ${PROJECT_SOURCE_DIR}/src/Core)
set_property(TARGET TerraGame PROPERTY CXX_STANDARD 17)
set_target_properties(TerraGame PROPERTIES
    MACOSX_RPATH ON
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "@loader_path/libraries"
    INSTALL_NAME_DIR "@loader_path/libraries"
)



# Create executable file
add_executable(Terra src/main.cpp)
set_property(TARGET Terra PROPERTY CXX_STANDARD 17)


# Copy assets to output directory
file(COPY "${CMAKE_SOURCE_DIR}/src/assets" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Set include directories for libraries
target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/include ${GLFW3_INCLUDE_DIR})
target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/lib ${GLFW3_LIBRARY})
target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/src ${GLFW3_LIBRARY})
target_include_directories(TerraGame PUBLIC ${PROJECT_SOURCE_DIR}/include ${GLFW3_INCLUDE_DIR})
target_include_directories(TerraGame PUBLIC ${PROJECT_SOURCE_DIR}/lib ${GLFW3_LIBRARY})


# Link these libraries to the target
target_link_libraries(Terra PRIVATE
    ${GLFW3_LIBRARY}
    OpenGL::GL
    TerraGame
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework QuartzCore"
)
# Link these libraries to the target
target_link_libraries(TerraGame PRIVATE
    ${GLFW3_LIBRARY}
    OpenGL::GL
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework QuartzCore"
)

# Copy libglfw.dylib next to your binary at runtime
add_custom_command(TARGET Terra POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${GLFW3_LIBRARY}
        $<TARGET_FILE_DIR:Terra>/libraries/
)
