#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>

class Shader;

#define TEXTURE_CACHE 2 //for offTexture

class Renderer {

public:
	static enum class BlendType { BLEND_ADDITIVE, BLEND_MULTIPLY, BLEND_ALPHA };
	static enum class GeometryType { SQAURE, CUBE };

private:

	static Renderer* instance;

	GLuint frameBuffer;
	GLuint depthBuffer;
	
	int resolutionWidth;
	int resolutionHeight;

	int renderWidth;
	int renderHeight;
	int renderQuality;

	float renderRatio;

	int offTextureIndex;
	GLuint OffTextureID[TEXTURE_CACHE];

	int colourBufferSetting;

	//Mesh* bindedMesh;
	Shader* bindedShader;
	//Texture* bindedTexture;
	int bindedTexture;

	GLuint shaderProgramID;
	GLuint sampler2d_Loc;
	GLuint uMvpMatrix_Loc;

	GLuint squareVAO;
	unsigned int squareVBO;
	unsigned int squareEBO;

	void(*renderGeometry)();

private:
	Renderer(const int& resolutionWidth, const int& resolutionHeight);
	~Renderer();

	void initDefaultShader();
	void initDefaultGeometry();

	void bindDefaultShader(const glm::mat4& mvpMatrix);
	void bindDefaultGeometry();

	void swapOffTexture();

public:
	static Renderer* getInstance();

	static void start(const int& resolutionWidth, const int& resolutionHeight);
	static void end();

	void bindTexture(const int& textureID);
	void bindTextureCubemap(const int& cubemapID);
	void bindShader(Shader* const shader);
	void bindShader(const int& shaderProgramID);

	void render();
	void renderToScreen();

	void enableHallOfMirrors(const bool& hallOfMirror);

	void useWindow(GLFWwindow* const window);

	void setBlendType(const BlendType& blendType);
	void setRenderResolution(const int& resolutionWidth, const int& resolutionHeight);
	void setRenderQuality(const int& renderQuality);
	void setRenderGeometry(const GeometryType& objectType);

};