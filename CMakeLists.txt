cmake_minimum_required(VERSION 3.8)

# Set project name
project(CPUGA C CXX)

# Set C++ version
set(CXX_STANDARD 17)

# Set the folder where the executable is created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Required by Cool/App/cool.cmake
set(PATH_TO_COOL ${CMAKE_SOURCE_DIR}/Cool)

# Grab all the source files
file(GLOB_RECURSE MY_SOURCES
    src/*
    ${PATH_TO_COOL}/Log/src/*
)

# Create target executable
add_executable(${PROJECT_NAME}
    ${MY_SOURCES}
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CXX_STANDARD})

# Include Cool
include("${PATH_TO_COOL}/Log/IncludeMe.cmake")

# Recreate the folder architecture inside the Visual Studio solution (might work for other IDEs as well)
include("${PATH_TO_COOL}/CMake-Helpers/VS_helpers.cmake")
VS_RegisterFiles("${MY_SOURCES}")

# Set include directories
include_directories(
    src
)

# /////////////////////////////////////////////////////////////////////////////
# ////////////////////////// PRECOMPILED HEADER ! /////////////////////////////
# /////////////////////////////////////////////////////////////////////////////

# More infos on precompiled headers : https://www.youtube.com/watch?v=eSI4wctZUto&ab_channel=TheCherno
target_precompile_headers(${PROJECT_NAME} PRIVATE
    # put things like <vector> here
    # <string>
)