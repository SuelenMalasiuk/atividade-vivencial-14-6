#include "Sprite.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Sprite::Sprite(GLuint shader, const char* texturePath, int nAnimations, int nFrames)
    : shaderProgram(shader),
      nAnimations(nAnimations),
      nFrames(nFrames),
      position(0.0f, 0.0f),
      scale(1.0f, 1.0f),
      rotation(0.0f)
{
    setupMesh();
    loadTexture(texturePath);
}

Sprite::~Sprite() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &textureID);
}

void Sprite::setupMesh() {
    // Quad de 1x1 — vai ser escalado na model matrix
    float vertices[] = {
        // positions   // texture coords
        0.0f, 0.0f,    0.0f, 0.0f,
        1.0f, 0.0f,    1.0f, 0.0f,
        1.0f, 1.0f,    1.0f, 1.0f,
        0.0f, 1.0f,    0.0f, 1.0f
    };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Sprite::loadTexture(const char* path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        std::cout << "Loaded texture: " << path << " (" << width << "x" << height << ", channels: " << nrChannels << ")" << std::endl;

        textureWidth = width;
        textureHeight = height;

        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
}

void Sprite::Update(float deltaTime) {
    frameTimer += deltaTime;

    if (frameTimer >= frameDuration) {
        frameTimer -= frameDuration;
        iFrame = (iFrame + 1) % nFrames;
    }
}

void Sprite::Draw(const glm::mat4& projection) {
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f)); // centraliza
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    float ds = 1.0f / (float)nFrames;
    float dt = 1.0f / (float)nAnimations;

    float offsetS = iFrame * ds;
    float offsetT = iAnimation * dt;

    glUniform2f(glGetUniformLocation(shaderProgram, "offset"), offsetS, offsetT);
    glUniform2f(glGetUniformLocation(shaderProgram, "scaleUV"), ds, dt);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sprite::setAnimation(int animIndex) {
    if (animIndex >= 0 && animIndex < nAnimations) {
        iAnimation = animIndex;
        iFrame = 0;
    }
}

glm::vec2 Sprite::getPosition() const {
    return position;
}

void Sprite::setPosition(const glm::vec2& pos) {
    position = pos;
}

glm::vec2 Sprite::getScale() const {
    return scale;
}

void Sprite::setScale(const glm::vec2& scl) {
    scale = scl;
}