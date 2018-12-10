#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.hpp"
#include "resManager.hpp"

const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;

void processInput(GLFWwindow *window);

Game breakout(SCR_WIDTH, SCR_HEIGHT);

int main() {
    breakout.init();

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwWindowShouldClose(window);
    }
}
