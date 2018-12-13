#pragma once

#include "gameObj.hpp"

class BallObj : public GameObj {
public:
    GLfloat radius;
    GLboolean stuck;

    BallObj();
    BallObj(glm::vec2 position, GLfloat radius, glm::vec2 velocity, Texture sprite);
    glm::vec2 move(GLfloat dt, GLuint windowWidth);
    void reset(glm::vec2 position, glm::vec2 velocity);
};
