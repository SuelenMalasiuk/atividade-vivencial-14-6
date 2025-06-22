#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sprite.h"


GLFWwindow* window;
GLuint shaderProgram;
Sprite* player;

struct Layer {
    Sprite* sprite;
    float parallaxFactor;
    glm::vec2 offset;

    Layer(Sprite* s, float factor) : sprite(s), parallaxFactor(factor), offset(0.0f, 0.0f) {}
};

std::vector<Layer> backgroundLayers;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float playerSpeed = 300.0f; 

glm::mat4 projection;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window) {
    glm::vec2 movement(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement.y += playerSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement.y -= playerSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement.x -= playerSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement.x += playerSpeed * deltaTime;

    player->setPosition(player->getPosition() + movement);

    for (auto& layer : backgroundLayers) {
        layer.offset -= movement * layer.parallaxFactor;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void renderScene() {
    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& layer : backgroundLayers) {
        layer.sprite->setPosition(layer.offset);
        layer.sprite->Draw(projection);
    }

    player->Draw(projection);
}

GLuint compileShader(const char* vertexSrc, const char* fragmentSrc);

int main() {
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Parallax com Sprite", nullptr, nullptr);
    if (!window) {
        std::cerr << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD\n";
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const char* vertexShaderSrc = R"(
        #version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoord;

        uniform mat4 model;
        uniform mat4 projection;

        out vec2 TexCoord;

        uniform vec2 offset;
        uniform vec2 scaleUV;

        void main() {
            gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
            TexCoord = aTexCoord * scaleUV + offset;
        }
    )";

    const char* fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoord;

        uniform sampler2D spriteTexture;

        void main() {
            FragColor = texture(spriteTexture, TexCoord);
            if (FragColor.a < 0.1)
                discard;
        }
    )";

    shaderProgram = compileShader(vertexShaderSrc, fragmentShaderSrc);

    Sprite* ceu = new Sprite(shaderProgram, "textures/ceu.png", 1, 1);
    Sprite* chao1 = new Sprite(shaderProgram, "textures/chao1.png", 1, 1);
    Sprite* chao2 = new Sprite(shaderProgram, "textures/chao2.png", 1, 1);
    Sprite* chao3 = new Sprite(shaderProgram, "textures/chao3.png", 1, 1);

    backgroundLayers.push_back(Layer(ceu, 0.1f));
    backgroundLayers.push_back(Layer(chao1, 0.3f));
    backgroundLayers.push_back(Layer(chao2, 0.7f));
    backgroundLayers.push_back(Layer(chao3, 1.0f));

    ceu->setPosition(glm::vec2(400.0f, 00.0f));
    ceu->setScale(glm::vec2(576.0f, 324.0f)); 
    chao1->setPosition(glm::vec2(400.0f, 300.0f));
    chao1->setScale(glm::vec2(800.0f, 600.0f)); 
    chao2->setPosition(glm::vec2(400.0f, 300.0f));
    chao2->setScale(glm::vec2(800.0f, 600.0f)); 
    chao3->setPosition(glm::vec2(400.0f, 300.0f));
    chao3->setScale(glm::vec2(800.0f, 600.0f)); 


    player = new Sprite(shaderProgram, "textures/Spritesheet1.png", 1, 8);
    player->setPosition(glm::vec2(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f));
    player->setScale(glm::vec2(100.0f, 100.0f)); 

    projection = glm::ortho(0.0f, float(SCR_WIDTH), 0.0f, float(SCR_HEIGHT));

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        renderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete player;
    for (auto& layer : backgroundLayers) {
        delete layer.sprite;
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

GLuint compileShader(const char* vertexSrc, const char* fragmentSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Erro de compilação do vertex shader:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Erro de compilação do fragment shader:\n" << infoLog << std::endl;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Erro de linkagem do shader program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}