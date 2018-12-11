#include "game.hpp"

Game::Game(GLuint width, GLuint height) : width(width), height(height), keys(), state(GAME_ACTIVE) {}
Game::~Game() {}

void Game::init() {}
void Game::processInput(GLfloat dt) {}
void Game::update(GLfloat dt) {}
void Game::render() {}

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
