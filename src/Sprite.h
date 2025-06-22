#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

class Sprite {
public:
    Sprite(GLuint shader, const char* texturePath, int nAnimations, int nFrames);
    ~Sprite();

    void Update(float deltaTime);
    void Draw(const glm::mat4& projection);
    void setAnimation(int animIndex);

    // Getters e Setters para position
    glm::vec2 getPosition() const;
    void setPosition(const glm::vec2& pos);

    // Getters e Setters para scale
    glm::vec2 getScale() const;
    void setScale(const glm::vec2& scl);

private:
    void setupMesh();
    void loadTexture(const char* path);

    GLuint VAO, VBO, EBO;
    GLuint textureID;
    GLuint shaderProgram;

    int nAnimations;
    int nFrames;
    int iAnimation = 0;
    int iFrame = 0;

    float frameTimer = 0.0f;
    float frameDuration = 0.1f;

    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 scale = glm::vec2(1.0f, 1.0f);
    float rotation = 0.0f;

    // infos da textura (útil se você quiser usar tamanho real depois)
    int textureWidth = 0;
    int textureHeight = 0;
};