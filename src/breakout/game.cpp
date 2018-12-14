#include "game.hpp"
#include "resManager.hpp"
#include "fileSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

const float Game::BALL_RADIUS = 12.5f;
const glm::vec2 Game::INIT_BALL_VELOCITY = glm::vec2(100.0f, -350.0f);
const float Game::PLAYER_VELOCITY = 500.0f;
const glm::vec2 Game::PLAYER_SIZE = glm::vec2(150.0f, 25.0f);

void Game::printLevels() {
    for (auto level : levels) {
        for (auto brick : level.bricks) {
            std::cout << brick.destroyed << '\n';
        }
    }
}

Game::Game(GLuint width, GLuint height) : width(width), height(height), keys(), state(GAME_ACTIVE), curLevel(), levels() {}

Game::~Game() {
    delete spriteRenderer;
}

void Game::init() {
    // load sprite
    ResManager::loadShader("spriteShader", FileSystem::getPath("src/breakout/shaders/sprite.vert").c_str(), FileSystem::getPath("src/breakout/shaders/sprite.frag").c_str());
    ResManager::loadTexture("blockSolid", FileSystem::getPath("src/breakout/textures/block_solid.png").c_str());
    ResManager::loadTexture("block", FileSystem::getPath("src/breakout/textures/block.png").c_str());
    ResManager::loadTexture("background", FileSystem::getPath("src/breakout/textures/background.jpg").c_str());
    ResManager::loadTexture("paddle", FileSystem::getPath("src/breakout/textures/paddle.png").c_str());
    ResManager::loadTexture("ball", FileSystem::getPath("src/breakout/textures/awesomeface.png").c_str());

    // load levels
    GameLevel standard;
    standard.load(FileSystem::getPath("src/breakout/levels/one.lvl").c_str(), width, height * 0.5);
    levels.emplace_back(standard);
    curLevel = 1;

    // Configure sprite shader
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, -1.0f, 1.0f);
    Shader spriteShader = ResManager::getShader("spriteShader");
    spriteShader.use();
    spriteShader.setInt("image", 0);
    spriteShader.setMat4("projection", projection);
    spriteRenderer = new SpriteRenderer(spriteShader);

    // initialize player
    glm::vec2 playerPosn = glm::vec2(width * 0.5f - PLAYER_SIZE.x * 0.5f, height - PLAYER_SIZE.y);
    Texture playerTexture = ResManager::getTexture("paddle");
    player = new GameObj(playerPosn, PLAYER_SIZE, playerTexture);

    // initialize ball
    glm::vec2 ballPosn = playerPosn + glm::vec2(PLAYER_SIZE.x * 0.5f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
    Texture ballTexture = ResManager::getTexture("ball");
    ball = new BallObj(ballPosn, BALL_RADIUS, INIT_BALL_VELOCITY, ballTexture);
}

void Game::processInput(GLfloat dt) {
    if (state == GAME_ACTIVE) {
        // player movement
        GLfloat displament = PLAYER_VELOCITY * dt;
        GLfloat posnX = 0, ballX = 0;
        if (keys[GLFW_KEY_A] && player->position.x >= 0) {
            posnX = player->position.x - displament;
            ballX = ball->position.x - displament;
            if (posnX < 0) {
                posnX = 0;
            }
            player->position.x = posnX;

            if (ball->stuck) {
                ball->position.x = posnX + player->size.x * 0.5f - ball->radius;
            }
        }

        if (keys[GLFW_KEY_D] && player->position.x <= width - player->size.x) {
            posnX = player->position.x + displament;
            if (posnX > width - player->size.x) {
                posnX = width - player->size.x;
            }
            player->position.x = posnX;

            if (ball->stuck) {
                ball->position.x = posnX + player->size.x * 0.5f - ball->radius;
            }
        }

        // release ball
        if (keys[GLFW_KEY_SPACE]) {
            ball->stuck = false;
        }
    }
}

void Game::update(GLfloat dt) {
    ball->move(dt, width);
    doCollision();

    if (ball->position.y >= height) {
        reset();
    }
}

void Game::render() {
    if (state == GAME_ACTIVE) {
        // render background image
        Texture bgTexture = ResManager::getTexture("background");
        spriteRenderer->drawSprite(bgTexture, glm::vec2(0.0f, 0.0f), glm::vec2(width, height), 0.0f);
        levels.at(curLevel - 1).draw(*spriteRenderer);

        // render player
        player->draw(*spriteRenderer);

        // render ball
        ball->draw(*spriteRenderer);
    }
}

void Game::doCollision() {
    for (auto &brick : levels.at(curLevel - 1).bricks) {
        if (!brick.destroyed) {
            Collision curCollision = checkBallCollision(brick, *ball);

            if (std::get<0>(curCollision)) {
                if (!brick.isSolid) {
                    brick.destroyed = GL_TRUE;
                }

                Direction curDirection = std::get<1>(curCollision);
                glm::vec2 curVector = std::get<2>(curCollision);
                if (curDirection == LEFT || curDirection == RIGHT) {
                    ball->velocity.x = -ball->velocity.x;
                    GLfloat penetration = ball->radius - glm::abs(curVector.x);
                    if (curDirection == LEFT) {
                        ball->position.x -= penetration;
                    } else {
                        ball->position.x += penetration;
                    }
                } else {
                    ball->velocity.y = -ball->velocity.y;
                    GLfloat penetration = ball->radius - glm::abs(curVector.y);
                    if (curDirection == UP) {
                        ball->position.y -= penetration;
                    } else {
                        ball->position.y += penetration;
                    }
                }
            }
        }
    }

    if (!ball->stuck) {
        Collision playerCollision = checkBallCollision(*player, *ball);
        if (std::get<0>(playerCollision)) {
            GLfloat paddleCenter = player->position.x + player->size.x / 2;
            GLfloat distanceToCenter = (ball->position.x + ball->radius) - paddleCenter;
            GLfloat percentage = distanceToCenter / (player->size.x) / 2;
            GLfloat strength = 2.0f;
            glm::vec2 oldVelocity = ball->velocity;
            ball->velocity.x = INIT_BALL_VELOCITY.x * percentage * strength;
            ball->velocity.y = -1 * glm::abs(ball->velocity.y);
            ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
        }
    }
}

GameState Game::getCurState() {
    return state;
}

void Game::setCurState(GameState state) {
    this->state = state;
}

GLuint Game::getWidth() {
    return width;
}

GLuint Game::getHeight() {
    return height;
}

GLboolean Game::getKey(int key) {
    return keys[key];
}

void Game::setKey(int key, GLboolean val) {
    keys[key] = val;
}

Collision Game::checkBallCollision(GameObj &obj, BallObj &ball) {
    // return obj1.position.x + obj1.size.x >= obj2.position.x && obj2.position.x + obj2.size.x >= obj1.position.x &&
    //     obj1.position.y + obj1.size.y >= obj2.position.y && obj2.position.y + obj2.size.y >= obj1.position.y;

    glm::vec2 objHalfSize = glm::vec2(obj.size.x * 0.5f, obj.size.y * 0.5f);
    glm::vec2 objCenter = glm::vec2(obj.position.x + objHalfSize.x, obj.position.y + objHalfSize.y);
    glm::vec2 ballCenter = glm::vec2(ball.position + glm::vec2(ball.radius));
    glm::vec2 clampedDistance = glm::clamp(ballCenter - objCenter, -objHalfSize, objHalfSize);
    glm::vec2 ballToObjEdgeVec = (objCenter + clampedDistance) - ballCenter;

    if (glm::length(ballToObjEdgeVec) < ball.radius) {
        return std::make_tuple(GL_TRUE, vectorDirection(ballToObjEdgeVec), ballToObjEdgeVec);
    } else {
        return std::make_tuple(GL_FALSE, UP, glm::vec2());
    }
}

Direction Game::vectorDirection(glm::vec2 vector) {
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, -1.0f),
        glm::vec2(-1.0f, 0.0f)
    };

    Direction direction = UP;
    GLfloat maxDot = 0.0f;

    for (int i = 0; i < 4; ++i) {
        GLfloat vectorDot = glm::dot(compass[i], vector);
        if (vectorDot > maxDot) {
            maxDot = vectorDot;
            direction = (Direction)i;
        }
    }

    return direction;
}

void Game::reset() {
    // reset level
    levels[0].reset(width, height * 0.5f);
    // reset player
    player->position = glm::vec2(width * 0.5f - PLAYER_SIZE.x * 0.5f, height - PLAYER_SIZE.y);
    player->size = PLAYER_SIZE;
    // reset ball
    ball->reset(player->position + glm::vec2(PLAYER_SIZE.x * 0.5f - BALL_RADIUS, -BALL_RADIUS * 2.0f), INIT_BALL_VELOCITY);
}
