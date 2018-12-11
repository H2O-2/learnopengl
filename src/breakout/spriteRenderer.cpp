#include "spriteRenderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(Shader &shader) : shader(shader) {
    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::drawSprite(Texture &texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color) {
    glm::mat4 model;
    // translation
    model = glm::translate(model, glm::vec3(position, 0.0f));
    // rotation
    model = glm::translate(model, glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-size.x * 0.5f, -size.y * 0.5f, 0.0f));
    // scale
    model = glm::scale(model, glm::vec3(size, 1.0f));

    shader.use();
    shader.setMat4("model", model);
    shader.setVec3("spriteColor", color);

    // sprite texture
    texture.activeAndBind(GL_TEXTURE0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
    GLuint VBO;
    GLfloat vertices[] = {
        // Position // Texture
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
