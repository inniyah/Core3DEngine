# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(CoreSDL2Example)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC")

set(OpenGL_GL_PREFERENCE GLVND)

# Path to your Core engine root directory
set(CORE_ROOT "..")

#~ add_compile_options(-H)

# Include Core headers
include_directories(
    ${CORE_ROOT}/include
)

# Link directories for Core and dependencies
link_directories(
    ${CORE_ROOT}
)

# Find packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

# Executable target
add_executable(CoreSDL2Example main.cpp)

# Link libraries
target_link_libraries(CoreSDL2Example
    core
    ${SDL2_LIBRARIES}
    GLEW::GLEW
    OpenGL::GL
    IL
    ILU
    assimp
    pthread
    dl
)
