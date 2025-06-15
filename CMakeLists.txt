cmake_minimum_required(VERSION 3.10)

# Defina o nome do projeto e a versão
project(IsometricoDiamond VERSION 1.0)

# Defina o padrão para o C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Diretório de saída
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Localize os diretórios com as bibliotecas (ajuste se necessário)
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(SOIL2 REQUIRED)

# Adicione os arquivos fontes
add_executable(isometrico src/main.cpp)

# Linkar com as bibliotecas necessárias
target_link_libraries(isometrico OpenGL::GL GLFW3::GLFW SOIL2::SOIL2)

# Defina os diretórios de inclusão das bibliotecas
target_include_directories(isometrico PRIVATE 
    ${OPENGL_INCLUDE_DIRS} 
    ${GLFW3_INCLUDE_DIRS} 
    ${SOIL2_INCLUDE_DIRS})
s