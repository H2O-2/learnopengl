#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_s.h"
#include "texture.hpp"

class SpriteRenderer {
public:
    SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    void drawSprite(Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader shader;
    GLuint quadVAO;

    void initRenderData();
};
