#include "texture.h"

#include "system.h"

Texture::Texture(const int& width, const int& height) {

	//for empty texture

	this->width = width;
	this->height = height;

	System::initTexture(textureID, 1, this->width, this->height);

	textureType = TextureType::CUSTOM;

}

Texture::Texture(const GLchar* dir, const TextureType& textureType) {

	//for texture2d

	System::initTexture(dir, textureID, width, height, internalFormat, format);

	this->textureType = textureType;

}

// !!! TO BE REMOVED !!! //
Texture::Texture(const std::vector<std::string>& dir) {

	//for cubemap

	System::loadCubemapTexture(dir, textureID);

}

Texture::~Texture() {
	glDeleteTextures(1, &textureID);
}

int Texture::getWidth() {
	return width;

}

int Texture::getHeight() {
	return height;

}

GLint Texture::getInternalFormat() {
	return internalFormat;
}

GLenum Texture::getFormat() {
	return format;
}

void Texture::reSize(const GLsizei& width, const GLsizei& height) {

	glBindTexture(GL_TEXTURE_2D, textureID);

	this->width = width;
	this->height = height;

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

}

GLuint  Texture::getTextureID() {
	return textureID;
}