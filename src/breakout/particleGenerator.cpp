#include "particleGenerator.hpp"

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, GLuint amount) : shader(shader), texture(texture), amount(amount), fadeSpeed(2.5f), lastUsedParticle(0) {
    init();
}

void ParticleGenerator::update(GLfloat dt, GameObj &obj, GLuint newParticles, glm::vec2 offset) {
    for (int i = 0; i < newParticles; ++i) {
        GLuint unusedParticle = firstUnused();
        respawnParticle(particles.at(unusedParticle), obj, offset);
    }

    for (auto &particle : particles) {
        particle.life -= dt;
        if (particle.life > 0.0f) {
            particle.position += particle.velocity * dt;
            particle.color.a -= dt * fadeSpeed;
        }
    }
}

void ParticleGenerator::draw() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader.use();
    glBindVertexArray(VAO);
    for (auto particle : particles) {
        if (particle.life > 0.0f) {
            shader.setVec2("offset", particle.position);
            shader.setVec4("color", particle.color);
            texture.activeAndBind(GL_TEXTURE0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glBindVertexArray(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
    GLfloat particleVertices[] = {
        // Position // Texture
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)(2 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for (int i = 0; i < amount; ++i) {
        particles.emplace_back(Particle());
    }
}

GLuint ParticleGenerator::firstUnused() {
    for (GLuint i = 0; i < amount; ++i) {
        if (particles[i].life < 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (GLuint j = 0; j < lastUsedParticle; ++j) {
        if (particles[j].life < 0.0f) {
            lastUsedParticle = j;
            return j;
        }
    }

    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObj &obj, glm::vec2 offset) {
    GLfloat rPos = ((rand() % 100) - 50.0f) / 10.0f;
    GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);
    particle.position = obj.position + rPos + offset;
    particle.color = glm::vec4(glm::vec3(rColor), 1.0f);
    particle.life = 1.0f;
    particle.velocity = obj.velocity * 0.1f;
}
