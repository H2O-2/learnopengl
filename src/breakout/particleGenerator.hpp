#pragma once

#include <vector>

#include "gameObj.hpp"

struct Particle {
    glm::vec2 position, velocity;
    glm::vec4 color;
    GLfloat life;

    Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture texture, GLuint amount);
    void update(GLfloat dt, GameObj &obj, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f));
    void draw();
private:
    std::vector<Particle> particles;
    Shader shader;
    Texture texture;
    GLuint amount;
    GLuint VAO;
    GLuint lastUsedParticle;
    GLfloat fadeSpeed;

    void init();
    GLuint firstUnused();
    void respawnParticle(Particle &particle, GameObj &obj, glm::vec2 offset = glm::vec2(0.0f));
};
