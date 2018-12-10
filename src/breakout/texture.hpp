#pragma once

#include <glad/glad.h>

class Texture {
public:
    GLuint id;

    Texture(GLsizei width, GLsizei height, GLenum format, unsigned char *data, GLenum sWrap = GL_REPEAT, GLenum tWrap = GL_REPEAT, GLenum minFilter = GL_LINEAR, GLenum maxFilter = GL_LINEAR);
    ~Texture();
    GLsizei getWidth();
    GLsizei getHeight();
    void activeAndBind(GLenum texture);
private:
    GLsizei width;
    GLsizei height;
};
