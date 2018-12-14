#pragma once

#include <glm/glm.hpp>

#include "texture.hpp"
#include "spriteRenderer.hpp"

class GameObj {
public:
    glm::vec2 position, size, velocity;
    glm::vec3 color;
    GLfloat rotation;
    GLboolean isSolid;
    bool destroyed;
    Texture sprite;

    GameObj();
    GameObj(glm::vec2 position, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));
    virtual void draw(SpriteRenderer &renderer);
};
