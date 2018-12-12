#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "gameObj.hpp"
#include "spriteRenderer.hpp"

class GameLevel {
public:
    std::vector<GameObj> bricks;

    GameLevel();
    void load(const GLchar *file, GLuint width, GLuint height);
    void draw(SpriteRenderer &renderer);
    GLboolean isComplete();
private:
    static const unsigned int SOLID_BRICK = 1;
    static const unsigned int BRICK_1 = 2;
    static const unsigned int BRICK_2 = 3;
    static const unsigned int BRICK_3 = 4;
    static const unsigned int BRICK_4 = 5;

    void init(std::vector<std::vector<GLuint>> &tileData, GLuint width, GLuint height);
};
