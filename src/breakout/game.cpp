#include "game.hpp"
#include "resManager.hpp"
#include "fileSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

const glm::vec2 Game::PLAYER_SIZE = glm::vec2(150.0f, 25.0f);
const float Game::PLAYER_VELOCITY = 500.0f;

Game::Game(GLuint width, GLuint height) : width(width), height(height), keys(), state(GAME_ACTIVE), curLevel(), levels() {}

Game::~Game() {
    delete spriteRenderer;
}

void Game::init() {
    // load sprite
    ResManager::loadShader("spriteShader", FileSystem::getPath("src/breakout/shaders/sprite.vert").c_str(), FileSystem::getPath("src/breakout/shaders/sprite.frag").c_str());
    ResManager::loadTexture("blockSolid", FileSystem::getPath("src/breakout/textures/block_solid.png").c_str());
    ResManager::loadTexture("block", FileSystem::getPath("src/breakout/textures/block.png").c_str());
    ResManager::loadTexture("background", FileSystem::getPath("src/breakout/textures/background.jpg").c_str());
    ResManager::loadTexture("paddle", FileSystem::getPath("src/breakout/textures/paddle.png").c_str());

    // load levels
    GameLevel standard;
    standard.load(FileSystem::getPath("src/breakout/levels/one.lvl").c_str(), width, height * 0.5);
    levels.emplace_back(standard);
    curLevel = 1;

    // Configure sprite shader
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, -1.0f, 1.0f);
    Shader spriteShader = ResManager::getShader("spriteShader");
    spriteShader.use();
    spriteShader.setInt("image", 0);
    spriteShader.setMat4("projection", projection);
    spriteRenderer = new SpriteRenderer(spriteShader);

    // initialize player
    glm::vec2 playerPosn = glm::vec2(width * 0.5f - PLAYER_SIZE.x * 0.5f, height - PLAYER_SIZE.y);
    Texture playerTexture = ResManager::getTexture("paddle");
    player = new GameObj(playerPosn, PLAYER_SIZE, playerTexture);
}

void Game::processInput(GLfloat dt) {
    if (state == GAME_ACTIVE) {
        GLfloat displament = PLAYER_VELOCITY * dt;
        GLfloat posnX = 0;
        if (keys[GLFW_KEY_A] && player->position.x >= 0) {
            posnX = player->position.x - displament;
            if (posnX < 0) {
                posnX = 0;
            }
            player->position.x = posnX;
        } else if (keys[GLFW_KEY_D] && player->position.x <= width - player->size.x) {
            posnX = player->position.x + displament;
            if (posnX > width - player->size.x) {
                posnX = width - player->size.x;
            }
            player->position.x = posnX;
        }
    }
}

void Game::update(GLfloat dt) {}

void Game::render() {
    if (state == GAME_ACTIVE) {
        // render background image
        Texture bgTexture = ResManager::getTexture("background");
        spriteRenderer->drawSprite(bgTexture, glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
        levels.at(curLevel - 1).draw(*spriteRenderer);

        // render player
        player->draw(*spriteRenderer);
    }
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
