#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>
#include <vector>

class Texture {

public:
	enum class TextureType { CUSTOM, DIFFUSE, SPECULAR, NORMAL };

private:

	GLuint textureID;

	int width;
	int height;

	GLint internalFormat;
	GLenum format;

	TextureType textureType;

public:

	Texture(const int& width, const int& height);
	Texture(const GLchar* dir, const TextureType& textureType = TextureType::CUSTOM);
	Texture(const std::vector<std::string>& dir); // !!! TO BE REMOVED !!! //
	~Texture();

	void reSize(const GLsizei& width, const GLsizei& height);

	int getWidth();
	int getHeight();

	GLenum getFormat();
	GLint getInternalFormat();

	GLuint getTextureID();

};