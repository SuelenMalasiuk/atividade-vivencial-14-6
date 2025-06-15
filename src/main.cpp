#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int TILE_WIDTH = 128;
const int TILE_HEIGHT = 64;

int map[3][3] = {
    {1, 1, 4},
    {4, 1, 4},
    {4, 4, 1}
};

int selectedI = 1, selectedJ = 1;

GLuint tilesetTexture;

void loadTexture(const char* path) {
    int width, height, channels;
    unsigned char* image = stbi_load(path, &width, &height, &channels, 4);
    glGenTextures(1, &tilesetTexture);
    glBindTexture(GL_TEXTURE_2D, tilesetTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
}

void drawTile(int i, int j, int tileIndex) {
    float tx = (tileIndex * TILE_WIDTH) / (float)(TILE_WIDTH * 7); // 7 tiles
    float tx2 = tx + TILE_WIDTH / (float)(TILE_WIDTH * 7);

    float screenX = (i - j) * (TILE_WIDTH / 2.0f);
    float screenY = (i + j) * (TILE_HEIGHT / 2.0f);

    glPushMatrix();
    glTranslatef(screenX + 400, screenY + 100, 0); // posição centralizada

    glBegin(GL_QUADS);
    glTexCoord2f(tx, 0);           glVertex2f(-TILE_WIDTH / 2, 0);
    glTexCoord2f(tx2, 0);          glVertex2f(TILE_WIDTH / 2, 0);
    glTexCoord2f(tx2, 1);          glVertex2f(TILE_WIDTH / 2, TILE_HEIGHT);
    glTexCoord2f(tx, 1);           glVertex2f(-TILE_WIDTH / 2, TILE_HEIGHT);
    glEnd();

    glPopMatrix();
}

void drawMap() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) {
            int tileID = (i == selectedI && j == selectedJ) ? 6 : map[i][j]; // 6 = tile rosa
            drawTile(i, j, tileID);
        }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    if (key == GLFW_KEY_W) selectedJ--;
    if (key == GLFW_KEY_S) selectedJ++;
    if (key == GLFW_KEY_A) selectedI--;
    if (key == GLFW_KEY_D) selectedI++;
    if (key == GLFW_KEY_Q) { selectedI--; selectedJ--; }
    if (key == GLFW_KEY_E) { selectedI++; selectedJ--; }
    if (key == GLFW_KEY_Z) { selectedI--; selectedJ++; }
    if (key == GLFW_KEY_C) { selectedI++; selectedJ++; }

    // Limites do mapa
    if (selectedI < 0) selectedI = 0;
    if (selectedI > 2) selectedI = 2;
    if (selectedJ < 0) selectedJ = 0;
    if (selectedJ > 2) selectedJ = 2;
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Isometrico Diamond - OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    loadTexture("tileset.png");

    glEnable(GL_TEXTURE_2D);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawMap();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
