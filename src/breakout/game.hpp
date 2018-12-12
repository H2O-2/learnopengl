#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "spriteRenderer.hpp"
#include "gameLevel.hpp"
#include "gameObj.hpp"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {
public:
    Game(GLuint width, GLuint height);
    ~Game();

    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();

    GameState getCurState();
    void setCurState(GameState state);
    GLuint getWidth();
    GLuint getHeight();
    GLboolean getKey(int key);
    void setKey(int key, GLboolean val);
private:
    GameState state;
    GLboolean keys[1024];
    GLuint width, height;
    SpriteRenderer *spriteRenderer;

    GLuint curLevel;
    std::vector<GameLevel> levels;

    GameObj *player;

    static const float PLAYER_VELOCITY;
    static const glm::vec2 PLAYER_SIZE;
};
