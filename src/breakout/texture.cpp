#include "texture.hpp"

Texture::Texture() : id(), width(), height() {}

Texture::Texture(GLsizei width, GLsizei height, GLenum format, unsigned char *data, GLenum sWrap, GLenum tWrap, GLenum minFilter, GLenum maxFilter) : width(width), height(height) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {}

GLsizei Texture::getWidth() {
    return this->width;
}

GLsizei Texture::getHeight() {
    return this->height;
}

void Texture::activeAndBind(GLenum textureIndex) {
    glActiveTexture(textureIndex);
    glBindTexture(GL_TEXTURE_2D, this->id);
}
