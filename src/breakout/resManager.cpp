#include "resManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<string, Shader> ResManager::shaders;
std::map<string, Texture> ResManager::textures;

Shader ResManager::loadShader(string name, const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath) {
    Shader shader(vertexPath, fragmentPath, geometryPath);
    shaders[name] = shader;
    return shader;
}

Texture ResManager::loadTexture(string name, const GLchar *texturePath) {
    Texture texture = loadTextureFromFile(texturePath);
    textures[name] = texture;
    return texture;
}
Shader ResManager::getShader(string name) {
    return shaders[name];
}

Texture ResManager::getTexture(string name) {
    return textures[name];
}

void ResManager::clear() {
    for (auto shader : shaders) {
        glDeleteProgram(shader.second.shaderProgram);
    }

    for (auto texture : textures) {
        glDeleteTextures(1, &(texture.second.id));
    }
}

Texture ResManager::loadTextureFromFile(const GLchar *texturePath) {
    int width, height, nrComponent;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrComponent, 0);
    if (data) {
        GLenum format = 0;
        if (nrComponent == 1) {
            format = GL_RED;
        } else if (nrComponent == 3) {
            format = GL_RGB;
        } else if (nrComponent == 4) {
            format = GL_RGBA;
        }

        Texture texture(width, height, format, data);

        stbi_image_free(data);

        return texture;
    } else {
        std::cout << "Texture failed to load at path: " << texturePath << std::endl;
        stbi_image_free(data);

        // TEMPORARY SOLUTION
        Texture texture(width, height, GL_RGB, data);
        return texture;
    }
}
