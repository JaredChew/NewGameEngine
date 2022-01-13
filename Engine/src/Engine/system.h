#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <vector>
#include <string>

namespace System {

	bool initOpenGL();

	bool initTexture(GLuint& textureID, const GLsizei& size, const GLuint& resolutionWidth, const GLuint& resolutionHeight);
	bool initTexture(const GLchar* dir, GLuint& textureID, GLint& width, GLint& height, GLint& internalFormat, GLenum& format);

	bool initDepthBufferTexture(GLuint& textureID, const GLuint& resolutionWidth, const GLuint& resolutionHeight);

	bool initProgramObject_Shader(GLuint& programID, const GLuint& fragmentShader, const GLuint& vertexShade);

	bool checkIsFramebufferReady();

	bool loadShaderRaw(GLuint& shaderID, const GLenum& type, const char* shaderSrc);
	bool loadShaderFromFile(GLuint& shaderID, const GLenum& type, const char* path);

	void loadCubemapTexture(std::vector<std::string> facesPath, GLuint& textureID);

}