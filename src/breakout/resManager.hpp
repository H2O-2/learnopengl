#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "shader_s.h"
#include "texture.hpp"

using std::string;

class ResManager {
public:
    static std::map<string, Shader> shaders;
    static std::map<string, Texture> textures;

    static Shader loadShader(string name, const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath = nullptr);
    static Texture loadTexture(string name, const GLchar *texturePath);
    static Shader getShader(string name);
    static Texture getTexture(string name);
    static void clear();
private:
    ResManager();
    static Texture loadTextureFromFile(const GLchar *texturePath);
};
