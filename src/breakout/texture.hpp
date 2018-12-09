#pragma once

#include <glad/glad.h>

class Texture {
public:
    Texture(GLuint width, GLuint height, unsigned char *data);
    ~Texture();
    GLuint getTexture();
    GLuint getWidth();
    GLuint getHeight();
    void activeAndBind();
private:
    GLuint ID;
    GLuint width;
    GLuint height;
};
