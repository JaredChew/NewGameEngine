#include "system.h"

#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>

#include "logger.h"

#include <string>

bool System::initOpenGL() {

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		Logger::getInstance()->errorLog("Failed to initialize OpenGL context");
		return -1;
	}

	Logger::getInstance()->infoLog((char*)glGetString(GL_VERSION));
	Logger::getInstance()->infoLog(glfwGetVersionString());

	if (!GLAD_GL_VERSION_3_2) {

		Logger::getInstance()->errorLog("Your device must support OpenGL 3.2+");

		return false;

	}

	return true;

}

bool System::initTexture(GLuint& textureID, const GLsizei& size, const GLuint& resolutionWidth, const GLuint& resolutionHeight) {

	glGenTextures(size, &textureID);

	if (!textureID) {

		Logger::getInstance()->errorLog("Texture generate error: can't generate %i texture!", size);
		return false;

	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolutionWidth, resolutionHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;

}

bool System::initTexture(const GLchar* dir, GLuint& textureID, GLint& width, GLint& height, GLint& internalFormat, GLenum& format) {

	int channels = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* image = SOIL_load_image(dir, &width, &height, &channels, SOIL_LOAD_AUTO);

	if (!image) {

		Logger::getInstance()->errorLog("SOIL loading error: %s", SOIL_last_result());
		SOIL_free_image_data(image);

		return false;

	}

	glGenTextures(1, &textureID);

	if (!textureID) {

		Logger::getInstance()->errorLog("Texture generate error: can't generate a texture!");
		return false;

	}

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (channels) {

	case SOIL_LOAD_L:
		internalFormat = GL_RED;
		format = GL_RED;
		break;

	case SOIL_LOAD_RGB:
		internalFormat = GL_RGB;
		format = GL_RGB;
		break;

	case SOIL_LOAD_RGBA:
		internalFormat = GL_RGBA;
		format = GL_RGBA;
		break;

	}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnifying = near, linear = gradient
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // minifying = far, nearest = more pixel
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); // GL_LINEAR); // magnifying = near, linear = gradient
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //GL_NEAREST); // minifying = far, nearest = more pixel

	SOIL_free_image_data(image);

	return true;

}

bool System::initDepthBufferTexture(GLuint& textureID, const GLuint& resolutionWidth, const GLuint& resolutionHeight) {

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, resolutionWidth, resolutionHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true ? textureID != 0 : false;

}

bool System::initProgramObject_Shader(GLuint& programID, const GLuint& fragmentShader, const GLuint& vertexShader) {

	GLuint programObject = glCreateProgram();

	if (programObject == 0) { return false; }

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	//glBindAttribLocation(programObject, 0, "vPosition");
	//glBindAttribLocation(programObject, 1, "vColor");
	//glBindAttribLocation(programObject, 2, "vTexCoord");

	// Link the program
	glLinkProgram(programObject);

	GLint linked;

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if (!linked) {

		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {

			char infoLog[1024];

			glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);

			Logger::getInstance()->warningLog("Failed to link shader program %s", infoLog);

		}

		glDeleteProgram(programObject);

		return false;
	}

	// Store the program object
	programID = programObject;

	glDetachShader(programID, fragmentShader);
	glDetachShader(programID, vertexShader);

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;

}

bool System::checkIsFramebufferReady() {

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {

		switch (status) {

		case GL_FRAMEBUFFER_UNDEFINED:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_UNDEFINED");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_UNSUPPORTED");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			Logger::getInstance()->warningLog("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			break;

		}

		return false;

	}

	return true;

}

bool System::loadShaderRaw(GLuint& shaderID, const GLenum& type, const char* shaderSrc) {

	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader(type);

	if (shader == 0) { return 0; }

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {

		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {

			char infoLog[4096];

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

			Logger::getInstance()->warningLog(infoLog);

		}

		glDeleteShader(shader);

		return false;

	}

	shaderID = shader;

	return true;

}

bool System::loadShaderFromFile(GLuint& shaderID, const GLenum& type, const char* path) {

	std::ifstream sourceFile(path);

	if (sourceFile) {

		std::string shaderString;

		shaderString.assign((std::istreambuf_iterator< char >(sourceFile)), std::istreambuf_iterator< char >());

		const GLchar* shaderSource = shaderString.c_str();

		return System::loadShaderRaw(shaderID, type, shaderSource);

	}
	else {

		//Logger::getInstance()->warningLog(std::string("Unable to open file " + path));

	}

	return false;

}

void System::loadCubemapTexture(std::vector<std::string> facesPath, GLuint& textureID) {

	glGenTextures(1, &textureID);

	if (!textureID) {

		Logger::getInstance()->errorLog("Texture generate error: can't generate cubemap texture!");
		return;

	}

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	stbi_set_flip_vertically_on_load(false);

	for (unsigned int i = 0; i < facesPath.size(); i++) {

		int width = 0, height = 0;
		int channels = 0, internalFormat = 0, format = 0;

		unsigned char* image = SOIL_load_image(facesPath[i].c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

		if (!image) {

			Logger::getInstance()->errorLog("SOIL loading error: %s", SOIL_last_result());
			SOIL_free_image_data(image);

		}

		switch (channels) {

			case SOIL_LOAD_L:
				internalFormat = GL_RED;
				format = GL_RED;
				break;

			case SOIL_LOAD_RGB:
				internalFormat = GL_RGB;
				format = GL_RGB;
				break;

			case SOIL_LOAD_RGBA:
				internalFormat = GL_RGBA;
				format = GL_RGBA;
				break;

		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glDisable(GL_TEXTURE_CUBE_MAP);

}