#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#if __linux__
    #include <SDL2/SDL.h>
#else
    #include <SDL.h>
#endif

#include "shader_s.h"
#include "camera.h"
#include "fileSystem.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(1.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float curTime = 0.0f;

uint32_t VAO;
uint32_t indexNum;

void initSphere(const uint16_t& stacks, const uint16_t& sectors, const float& radius);

void outputError(const std::string& error) {
    std::cerr << error << '\n';
}

int main(int, char**) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        outputError("FAILED TO INIT SDL!");
        exit(-1);
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_Window* window = SDL_CreateWindow("PBR Lighting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT,
                                            SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        outputError("FAILED TO CREATE WINDOW!");
        SDL_Quit();
        exit(-1);
    }

    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    if (!gladLoadGL()) {
        outputError("FAIL TO INITIALIZE GLAD!");
        SDL_Quit();
        exit(-1);
    }

    int framebufferWidth, framebufferHeight;
    SDL_GL_GetDrawableSize(window, &framebufferWidth, &framebufferHeight);
    std::cout << framebufferHeight << '\n';
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glEnable(GL_DEPTH_TEST);

    Shader pbr(FileSystem::getPath("src/pbrLighting.vert").c_str(), FileSystem::getPath("src/pbrLighting.frag").c_str());
    pbr.use();

    initSphere(32, 64, 1.0);

    bool quit = false;
    while (!quit) {
        float newTime = SDL_GetTicks() * 0.001f;
        deltaTime = newTime - curTime;
        curTime = newTime;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_CLOSE:
                            if (event.window.windowID == SDL_GetWindowID(window)) {
                                quit = true;
                            }
                            break;
                        case SDL_WINDOWEVENT_RESIZED:
                            int framebuffer_width, framebuffer_height;
                            SDL_GL_GetDrawableSize(window, &framebuffer_width, &framebuffer_height);
                            glViewport(0, 0, framebuffer_width, framebuffer_height);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_w:
                            camera.ProcessKeyboard(FORWARD, deltaTime);
                            break;
                        case SDLK_a:
                            camera.ProcessKeyboard(LEFT, deltaTime);
                            break;
                        case SDLK_s:
                            camera.ProcessKeyboard(BACKWARD, deltaTime);
                            break;
                        case SDLK_d:
                            camera.ProcessKeyboard(RIGHT, deltaTime);
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    camera.ProcessMouseMovement(event.motion.xrel, event.motion.yrel);
                    break;
                case SDL_MOUSEWHEEL:
                    camera.ProcessMouseScroll(event.wheel.y);
                    break;
                default:
                    break;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 50.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model;

        pbr.setMat4("projection", projection);
        pbr.setMat4("view", view);
        pbr.setMat4("model", model);

        // std::cout << glGetError() << '\n';
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexNum, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }
}

// Referenced from http://www.songho.ca/opengl/gl_sphere.html
void initSphere(const uint16_t& stacks, const uint16_t& sectors, const float& radius) {
    uint32_t VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    std::vector<glm::vec3> posns;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<float> data;
    std::vector<uint32_t> indices;

    float x, y, z, xy; // posns

    float sectorStep = 2 * M_PI / (float)sectors;
    float stackStep = M_PI / (float)stacks;
    float sectorAngle, stackAngle;

    // Note that the starting point and ending point have the same posn but different texture coordinate
    for (uint16_t j = 0; j < stacks + 1; ++j) {
        stackAngle = j * stackStep;
        xy = radius * sinf(stackAngle);
        z = radius * cosf(stackAngle);

        for (uint16_t i = 0; i < sectors + 1; ++i) {
            sectorAngle = i * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            posns.emplace_back(x, y, z);
            texCoords.emplace_back((float)i / (float)sectors, (float)j / (float)stacks);
            normals.emplace_back(x, y, z);
        }
    }

    uint32_t curStackIndex, nextStackIndex;
    for (int j = 0; j < stacks; ++j) {
        curStackIndex = j * (sectors + 1);
        nextStackIndex = curStackIndex + sectors + 1;

        for (int i = 0; i < sectors; ++i, ++curStackIndex, ++nextStackIndex) {
            if (j != 0) {
                indices.emplace_back(curStackIndex);
                indices.emplace_back(nextStackIndex);
                indices.emplace_back(curStackIndex + 1);
            }

            if (j != (stacks - 1)) {
                indices.emplace_back(curStackIndex + 1);
                indices.emplace_back(nextStackIndex);
                indices.emplace_back(nextStackIndex + 1);
            }
        }
    }

    indexNum = indices.size();

    for (int i = 0; i < posns.size(); ++i) {
        data.emplace_back(posns[i].x);
        data.emplace_back(posns[i].y);
        data.emplace_back(posns[i].z);

        if (normals.size() > 0) {
            data.emplace_back(normals[i].x);
            data.emplace_back(normals[i].y);
            data.emplace_back(normals[i].z);
        }

        if (texCoords.size() > 0) {
            data.emplace_back(texCoords[i].x);
            data.emplace_back(texCoords[i].y);
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
}
