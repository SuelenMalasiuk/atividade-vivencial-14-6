cmake_minimum_required(VERSION 3.10)
project(SpriteScene)

set(CMAKE_CXX_STANDARD 17)

# Incluir os headers
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/glad
    ${CMAKE_SOURCE_DIR}/include/glfw
    ${CMAKE_SOURCE_DIR}/include/glm
)

# Diretório da lib
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Adiciona o executável
add_executable(SpriteScene
    src/main.cpp
    src/Sprite.cpp
    include/glad/glad.c
)

# Linka com as libs necessárias (completa para Windows)
target_link_libraries(SpriteScene
    glfw3
    opengl32
    gdi32
    user32
    kernel32
)