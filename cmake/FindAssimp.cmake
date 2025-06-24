# Copyright (c) 2015 Andrew Kelley
# This file is MIT licensed.
# See http://opensource.org/licenses/MIT

# ASSIMP_FOUND
# ASSIMP_INCLUDE_DIR
# ASSIMP_LIBRARY

# Find include files
find_path(
        ASSIMP_INCLUDE_DIR
        NAMES assimp/scene.h
        PATHS
        ${PROJECT_SOURCE_DIR}/include
        DOC "The directory where assimp/scene.h resides")

# Find library files
find_library(
        ASSIMP_LIBRARY
        NAMES assimp
        PATHS
        ${PROJECT_SOURCE_DIR}/lib
        DOC "The Assimp library")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Assimp DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)

mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)