#include "gameObj.hpp"

GameObj::GameObj() : position(glm::vec2(0.0f)), size(glm::vec2(1.0f)), velocity(glm::vec2(0.0f)), color(glm::vec3(1.0f)), rotation(0.0f), sprite(), isSolid(false), destroyed(false) {}

GameObj::GameObj(glm::vec2 position, glm::vec2 size, Texture sprite, glm::vec3 color, glm::vec2)
    : position(position), size(size), sprite(sprite), color(color), velocity(velocity), rotation(0.0f), isSolid(false), destroyed(false) {}

void GameObj::draw(SpriteRenderer &renderer) {
    renderer.drawSprite(sprite, position, size, rotation, color);
}
