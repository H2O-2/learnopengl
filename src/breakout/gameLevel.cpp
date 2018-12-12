#include "gameLevel.hpp"
#include "resManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

GameLevel::GameLevel() : bricks() {}

void GameLevel::load(const GLchar *file, GLuint width, GLuint height) {
    // clear old level
    bricks.clear();
    // read from file
    std::ifstream levelFile;
    std::vector<std::vector<GLuint>> tileData;

    levelFile.exceptions(std::ifstream::badbit);

    try {
        levelFile.open(file);

        std::string s;
        GLuint tileCode;
        while (std::getline(levelFile, s)) {
            std::stringstream ss(s);
            std::vector<GLuint> row;
            while (ss >> tileCode) {
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }

        levelFile.close();

        if (tileData.size() > 0) {
            init(tileData, width, height);
        }
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::LEVEL::FILE_NOT_SUCCESSFULLY_READ: " << std::endl;
        std::cout << strerror(errno) << '\n';
    }
}

void GameLevel::draw(SpriteRenderer &renderer) {
    for (auto tile : bricks) {
        if (!tile.destroyed) {
            tile.draw(renderer);
        }
    }
}

GLboolean GameLevel::isComplete() {
    for (auto tile : bricks) {
        if (!tile.isSolid && !tile.destroyed) {
            return GL_FALSE;
        }
    }

    return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> &tileData, GLuint width, GLuint height) {
    GLuint rowNum = tileData.size();
    GLuint columnNum = tileData.at(0).size();
    GLfloat brickWidth = (GLfloat)width / (GLfloat)columnNum;
    GLfloat brickHeight = (GLfloat)height / (GLfloat)rowNum;

    for (GLuint i = 0; i < rowNum; ++i) {
        for (GLuint j = 0; j < columnNum; ++j) {
            GLuint curTile = tileData[i][j];
            glm::vec2 posn = glm::vec2(brickWidth * j, brickHeight * i);
            glm::vec2 size = glm::vec2(brickWidth, brickHeight);
            GameObj gameObj;
            Texture blockTexture = ResManager::getTexture("block");
            if (curTile == SOLID_BRICK) {
                gameObj = GameObj(posn, size, ResManager::getTexture("blockSolid"), glm::vec3(1.0f));
            } else if (curTile == BRICK_1) {
                gameObj = GameObj(posn, size, blockTexture, glm::vec3(0.2f, 0.6f, 1.0f));
            } else if (curTile == BRICK_2) {
                gameObj = GameObj(posn, size, blockTexture, glm::vec3(0.0f, 0.7f, 0.0f));
            } else if (curTile == BRICK_3) {
                gameObj = GameObj(posn, size, blockTexture, glm::vec3(0.8f, 0.8f, 0.4f));
            } else if (curTile == BRICK_4) {
                gameObj = GameObj(posn, size, blockTexture, glm::vec3(1.0f, 0.5f, 0.5f));
            }

            bricks.emplace_back(gameObj);
        }
    }
}
