

# GLFW_FOUND
# GLFW_INCLUDE_DIR
# GLFW_LIBRARY

find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h PATHS ${CMAKE_SOURCE_DIR}/include)

find_library(GLFW_LIBRARY
    NAMES glfw3 glfw
    PATHS ${CMAKE_SOURCE_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIR)

if(GLFW_FOUND)
    set(GLFW_LIBRARIES ${GLFW_LIBRARY})
    set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
endif()