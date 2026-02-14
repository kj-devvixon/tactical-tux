cmake_minimum_required(VERSION 3.14)
project(TacticalTux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- GLM ---
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG master)

# --- GLFW ---
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# --- GLEW (Using the CMake-friendly mirror) ---
FetchContent_Declare(
    glew
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
    GIT_TAG master
)

# Crucial: This makes the variables glew_SOURCE_DIR available immediately
FetchContent_MakeAvailable(glm glfw glew)

find_package(OpenGL REQUIRED)

# --- Build Configuration ---
set(SOURCES
    src/main.cpp src/Camera.cpp src/Shader.cpp src/Mesh.cpp
    src/Player.cpp src/Physics.cpp src/Crosshair.cpp src/MapEditor.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Point to your headers AND the downloaded GLEW/GLM headers
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${glew_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
)

# Define GLEW_STATIC because we are linking the static library target
target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    glfw
    libglew_static  # This is the target name from glew-cmake
)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
