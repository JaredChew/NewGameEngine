#include "renderer.h"

#include "system.h"
#include "shader.h"
#include "geometry.h"

#include <glm/gtc/type_ptr.hpp>

Renderer* Renderer::instance;

Renderer::Renderer(const int& resolutionWidth, const int& resolutionHeight) {

	renderQuality = 100;
	bindedTexture = 0;
	offTextureIndex = 0;

	shaderProgramID = -1;
	sampler2d_Loc = -1;
	uMvpMatrix_Loc = -1;

	squareVAO = 0;
	squareVBO = 0;
	squareEBO = 0;

	bindedShader = nullptr;

	colourBufferSetting = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

	renderGeometry = &DefaultGeometry::generateSquare;

	//Init Opengl state
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	glClearDepth(1.0f); // Depth Buffer Setup

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	// The Type Of Depth Testing To Do

	//wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glLineWidth(2.5f);

	glEnable(GL_BLEND);

	setBlendType(BlendType::BLEND_ALPHA);

	glViewport(0, 0, resolutionWidth, resolutionHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	this->resolutionWidth = renderWidth = resolutionWidth;
	this->resolutionHeight = renderHeight = resolutionHeight;

	renderRatio = resolutionWidth / resolutionHeight;

	glGenFramebuffers(1, &frameBuffer);

	System::initDepthBufferTexture(depthBuffer, resolutionWidth, resolutionHeight);

	for(int i = 0; i < TEXTURE_CACHE; i++) {

		System::initTexture(OffTextureID[i], 1, resolutionWidth, resolutionHeight);

	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, OffTextureID[offTextureIndex], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	initDefaultShader();
	initDefaultGeometry();

}

Renderer::~Renderer() { }

void Renderer::initDefaultShader() {

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	const char* fragment =

		"#version 330 core\n"

		"in vec2 fTexCoord;\n"
		"in vec4 fColor;\n"
		"in vec4 fPosition;\n"

		"uniform sampler2D texture2D;\n"

		"void main() {\n"

		"gl_FragColor = texture(texture2D, fTexCoord);\n"

		"}"

		;

	const char* vertex =

		"#version 330 core\n"

		"layout(Location = 0) in vec3 vPosition;\n"
		"layout(Location = 1) in vec4 vColor;\n"
		"layout(Location = 2) in vec2 vTexCoord;\n"

		"out vec2 fTexCoord;\n"
		"out vec4 fColor;\n"
		"out vec4 fPosition;\n"

		"uniform mat4 uMvpMatrix;\n"

		"void main() {\n"

		"fColor = vColor;\n"
		"fTexCoord = vTexCoord;\n"
		"fPosition = vec4(vPosition, 1.0f);\n"

		"gl_Position = uMvpMatrix * vec4(vPosition, 1.0f);\n"

		"}"

		;

	if (!System::loadShaderRaw(vertexShaderID, GL_VERTEX_SHADER, vertex)) { printf("here\n\n"); }
	if (!System::loadShaderRaw(fragmentShaderID, GL_FRAGMENT_SHADER, fragment)) { printf("here2\n\n"); }

	if (!System::initProgramObject_Shader(shaderProgramID, fragmentShaderID, vertexShaderID)) {
		return;
	}

	glUseProgram(shaderProgramID);

	sampler2d_Loc = glGetUniformLocation(shaderProgramID, "texture2D");
	uMvpMatrix_Loc = glGetUniformLocation(shaderProgramID, "uMvpMatrix");

	if (sampler2d_Loc == -1) { printf("SHADER_PROGRAM_DEFAULT2 uniform \"texture2D\" not found"); }
	if (uMvpMatrix_Loc == -1) { printf("SHADER_PROGRAM_DEFAULT2 uniform \"uMvpMatrix\" not found"); }

	glUseProgram(0);

}

void Renderer::initDefaultGeometry() {

	GLuint squareIndices[] = {

		0, 1, 2,
		2, 3, 0

	};

	GLfloat squareVertices[] = {

		-1.0f,  1.0f, 0.0f, //top left
		-1.0f, -1.0f, 0.0f, //bottom left
		 1.0f, -1.0f, 0.0f, //bottom right
		 1.0f,  1.0f, 0.0f, //top right

	};

	GLfloat squareColors[] = {

		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f

	};

	GLfloat squareTexCoords[] = {

		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,

	};

	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);

	if (squareVBO == 0) {

		glGenBuffers(1, &squareVBO);
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices) + sizeof(squareColors) + sizeof(squareTexCoords), squareVertices, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(squareVertices), squareVertices);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices), sizeof(squareColors), squareColors);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices) + sizeof(squareColors), sizeof(squareTexCoords), squareTexCoords);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, (void*)sizeof(squareVertices));
		glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, (void*)(sizeof(squareVertices) + sizeof(squareColors)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

	}

	if (squareEBO == 0) {

		glGenBuffers(1, &squareEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Renderer::bindDefaultShader(const glm::mat4& mvpMatrix) {

	glUseProgram(shaderProgramID);

	glUniform1i(sampler2d_Loc, 0);
	glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

}

void Renderer::bindDefaultGeometry() {

	glBindVertexArray(squareVAO);

}

void Renderer::swapOffTexture() {

	glClear(colourBufferSetting);

	offTextureIndex = ++offTextureIndex % TEXTURE_CACHE;

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, OffTextureID[offTextureIndex], 0);

}

Renderer* Renderer::getInstance() {

	return instance;

}

void Renderer::start(const int& resolutionWidth, const int& resolutionHeight) {

	if (!instance) { instance = new Renderer(resolutionWidth, resolutionHeight); }

}

void Renderer::end() {

	if (instance) { delete instance; }

}

void Renderer::render() {

	if (!System::checkIsFramebufferReady()) { return; }

	(*renderGeometry)();
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //find a way to get total mesh triangles
	
}

void Renderer::renderToScreen() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (System::checkIsFramebufferReady()) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, OffTextureID[offTextureIndex]);

		bindDefaultShader(glm::mat4(1.0f));
		bindDefaultGeometry();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindVertexArray(0);

	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	swapOffTexture();

}

void Renderer::enableHallOfMirrors(const bool& hallOfMirror) {

	if (hallOfMirror) { colourBufferSetting = GL_DEPTH_BUFFER_BIT; }
	else { colourBufferSetting = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT; }

}

void Renderer::bindTexture(const int& textureID) {

	bindedTexture = textureID;

	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Renderer::bindTextureCubemap(const int& cubemapID) {

	bindedTexture = cubemapID;

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

}

void Renderer::bindShader(Shader* const shader) {

	if (bindedShader != nullptr) { bindedShader->isBinded = false; }

	bindedShader = shader;

	glUseProgram(shader->getShaderProgramID());

	shader->isBinded = true;

}

void Renderer::bindShader(const int& shaderProgramID) {

	glUseProgram(shaderProgramID);

}

void Renderer::useWindow(GLFWwindow* const window) {

	glfwMakeContextCurrent(window);

}

void Renderer::setBlendType(const BlendType& blendType) {

	switch (blendType) {

		case BlendType::BLEND_ADDITIVE:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);
			break;

		case BlendType::BLEND_MULTIPLY:
			glBlendFunc(GL_DST_COLOR, GL_ZERO);
			glBlendEquation(GL_FUNC_ADD);
			break;

		case BlendType::BLEND_ALPHA:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
			break;

	}

}

void Renderer::setRenderResolution(const int& resolutionWidth, const int& resolutionHeight) {

	this->resolutionWidth = resolutionWidth;
	this->resolutionHeight = resolutionHeight;

	setRenderQuality(renderQuality);

}

void Renderer::setRenderQuality(const int& renderQuality) {

	this->renderQuality = renderQuality;

	if (this->renderQuality <= 0) { this->renderQuality = 1; }

	renderWidth = resolutionWidth * (renderQuality / 100);
	renderHeight = resolutionHeight * (renderQuality / 100);

	renderRatio = renderWidth / renderHeight;

	glViewport(0, 0, renderWidth, renderHeight);

}

void Renderer::setRenderGeometry(const GeometryType& objectType) {

	switch (objectType) {

	case GeometryType::SQAURE:
		renderGeometry = &Geometry::drawSquare;
		break;

	case GeometryType::CUBE:
		renderGeometry = &Geometry::drawCube;
		break;

	}

}