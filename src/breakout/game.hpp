#pragma once

#include <glad/glad.h>

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
private:
    GameState state;
    GLboolean keys[1024];
    GLuint width, height;
};
