#include "ballObj.hpp"

BallObj::BallObj() : GameObj(), radius(12.5f), stuck(true) {}

BallObj::BallObj(glm::vec2 position, GLfloat radius, glm::vec2 velocity, Texture sprite) : GameObj(position, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), radius(12.5f), stuck(true) {}

glm::vec2 BallObj::move(GLfloat dt, GLuint windowWidth) {
    if (!stuck) {
        position += velocity * dt;

        if (position.x <= 0.0f) {
            velocity.x = -velocity.x;
            position.x = 0.0f;
        } else if (position.x + size.x >= windowWidth) {
            velocity.x = -velocity.x;
            position.x = windowWidth - size.x;
        }

        if (position.y <= 0.0f) {
            velocity.y = -velocity.y;
            position.y = 0.0f;
        }
    }

    return position;
}

void BallObj::reset(glm::vec2 position, glm::vec2 velocity) {
    this->position = position;
    this->velocity = velocity;
    stuck = true;
}
