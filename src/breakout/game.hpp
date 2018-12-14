#pragma once

#include <vector>
#include <tuple>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "spriteRenderer.hpp"
#include "gameLevel.hpp"
#include "ballObj.hpp"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game {
public:
    Game(GLuint width, GLuint height);
    ~Game();

    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();
    void doCollision();

    GameState getCurState();
    void setCurState(GameState state);
    GLuint getWidth();
    GLuint getHeight();
    GLboolean getKey(int key);
    void setKey(int key, GLboolean val);
    void printLevels();
private:
    static const float BALL_RADIUS;
    static const glm::vec2 INIT_BALL_VELOCITY;
    static const float PLAYER_VELOCITY;
    static const glm::vec2 PLAYER_SIZE;

    GameState state;
    GLboolean keys[1024];
    GLuint width, height;
    SpriteRenderer *spriteRenderer;

    GLuint curLevel;
    std::vector<GameLevel> levels;

    GameObj *player;
    BallObj *ball;

    Collision checkBallCollision(GameObj &obj, BallObj &ball);
    Direction vectorDirection(glm::vec2 vector);
    void reset();
};
