cmake_minimum_required(VERSION 3.10)
project(FluidSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Find GLM
find_path(GLM_INCLUDE_DIR glm/glm.hpp
    HINTS
    /usr/include
    /usr/local/include
    /opt/homebrew/include
)

if(NOT GLM_INCLUDE_DIR)
    message(FATAL_ERROR "GLM not found. Please install GLM library.")
endif()

# Include directories
include_directories(${GLM_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

# Create executable
add_executable(fluid_simulation fluid_sim.cpp)

# Link libraries
target_link_libraries(fluid_simulation 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(fluid_simulation PRIVATE -O3 -march=native)
endif()

# macOS specific settings
if(APPLE)
    target_link_libraries(fluid_simulation "-framework OpenGL")
endif()
