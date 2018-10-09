#ifndef SHADER_S_H
#define SHADER_S_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include <glm/gtc/type_ptr.hpp>

using std::string;

class Shader {
public:
    unsigned int shaderProgram;

    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use();
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
private:
    void checkCompileErrors(unsigned int shader, string type);
    void debug(const string s) const;
};

#endif
