
cmake_minimum_required(VERSION 3.12)
project(Terra VERSION 1.0
    DESCRIPTION "OpenGL engine project"
    LANGUAGES C CXX)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 

add_compile_definitions(ASSET_DIR="./assets")

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Terra src/main.cpp src/glad.c ${SOURCES})
set_property(TARGET Terra PROPERTY CXX_STANDARD 17)


file(COPY "${CMAKE_SOURCE_DIR}/src/assets" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/lib)

target_link_libraries(Terra PRIVATE glfw OpenGL::GL)
