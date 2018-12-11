#include "game.hpp"
#include "resManager.hpp"
#include "fileSystem.h"

#include <glm/gtc/matrix_transform.hpp>

Game::Game(GLuint width, GLuint height) : width(width), height(height), keys(), state(GAME_ACTIVE) {}

Game::~Game() {
    delete spriteRenderer;
}

void Game::init() {
    ResManager::loadShader("spriteShader", FileSystem::getPath("src/breakout/shaders/sprite.vert").c_str(), FileSystem::getPath("src/breakout/shaders/sprite.frag").c_str());
    ResManager::loadTexture("test", FileSystem::getPath("src/breakout/textures/awesomeface.png").c_str());

    // Configure sprite shader
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, -1.0f, 1.0f);
    Shader spriteShader = ResManager::getShader("spriteShader");
    spriteShader.use();
    spriteShader.setInt("image", 0);
    spriteShader.setMat4("projection", projection);
    spriteRenderer = new SpriteRenderer(spriteShader);
}

void Game::processInput(GLfloat dt) {}

void Game::update(GLfloat dt) {}

void Game::render() {
    Texture texture = ResManager::getTexture("test");
    spriteRenderer->drawSprite(texture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

GameState Game::getCurState() {
    return state;
}

void Game::setCurState(GameState state) {
    this->state = state;
}

GLuint Game::getWidth() {
    return width;
}

GLuint Game::getHeight() {
    return height;
}

GLboolean Game::getKey(int key) {
    return keys[key];
}

void Game::setKey(int key, GLboolean val) {
    keys[key] = val;
}
