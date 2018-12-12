#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game.hpp"
#include "resManager.hpp"

const GLuint SCR_WIDTH = 1280;
const GLuint SCR_HEIGHT = 720;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

Game breakout(SCR_WIDTH, SCR_HEIGHT);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Breakout", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize glad\n";
        return -1;
    }

#ifdef __APPLE__
    glViewport(0, 0, 2 * SCR_WIDTH, 2 * SCR_HEIGHT);
#else
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
#endif
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    breakout.init();

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        breakout.processInput(deltaTime);
        breakout.update(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        breakout.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // std::cout << glGetError() << '\n';

    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key > 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            breakout.setKey(key, GL_TRUE);
        } else if(action == GLFW_RELEASE) {
            breakout.setKey(key, GL_FALSE);
        }
    }
}
