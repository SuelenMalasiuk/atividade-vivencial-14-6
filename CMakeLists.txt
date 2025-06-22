cmake_minimum_required(VERSION 3.10)
project(SpriteScene)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glad
    ${CMAKE_SOURCE_DIR}/include/glfw
    ${CMAKE_SOURCE_DIR}/include/glm
)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(SpriteScene
    src/main.cpp
    src/Sprite.cpp
    include/glad/glad.c
)

target_link_libraries(SpriteScene
    glfw3
    opengl32
    gdi32
    user32
    kernel32
)