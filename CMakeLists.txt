
cmake_minimum_required(VERSION 3.12)
project(Terra VERSION 1.0
    DESCRIPTION "OpenGL engine project"
    LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)

# message(STATUS "GLFW_LIBRARIES: ${GLFW_LIBRARIES}")
# message(STATUS "GLFW_INCLUDE_DIRS: ${GLFW_INCLUDE_DIRS}")


# Set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 


# Change assets dir to be defined as local one
add_compile_definitions(ASSET_DIR="assets")

# Assign all sources to SOURCES list
file(GLOB_RECURSE SOURCES "src/*.cpp")
# Create executable file
add_executable(Terra src/main.cpp src/glad.c ${SOURCES})
set_property(TARGET Terra PROPERTY CXX_STANDARD 17)


# Copy assets to output directory
file(COPY "${CMAKE_SOURCE_DIR}/src/assets" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Set include directories for libraries
target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/include ${GLFW_INCLUDE_DIRS})
target_include_directories(Terra PUBLIC ${PROJECT_SOURCE_DIR}/lib ${GLFW_LIBRARIES})

# Link these libraries to the target
target_link_libraries(Terra PRIVATE
    ${GLFW_LIBRARIES}
    OpenGL::GL
    # "-framework Cocoa"
    # "-framework IOKit"
    # "-framework CoreVideo"
    # "-framework QuartzCore"
)

# Copy libglfw.dylib next to your binary at runtime
add_custom_command(TARGET Terra POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${GLFW_LIBRARIES}
        $<TARGET_FILE_DIR:Terra>
)
