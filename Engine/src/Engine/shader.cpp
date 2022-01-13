#include "shader.h"

#include "system.h"
#include "logger.h"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* fragmentPath, const char* vertexPath) {

	isBinded = false;

	GLuint vertexShaderID;
	GLuint fragmentShaderID;

	System::loadShaderFromFile(vertexShaderID, GL_VERTEX_SHADER, vertexPath);
	System::loadShaderFromFile(fragmentShaderID, GL_FRAGMENT_SHADER, fragmentPath);

	if (!System::initProgramObject_Shader(shaderProgramID, fragmentShaderID, vertexShaderID)) {
		Logger::getInstance()->warningLog("Failed to init \"ShaderName\" shader program");
		return;
	}

}

Shader::~Shader() {

	glDeleteProgram(shaderProgramID);

}

void Shader::init(const std::vector<std::string>& uniformName, const int& totalUniformVariables) {

	glUseProgram(shaderProgramID);

	for (int i = 0; i < totalUniformVariables; ++i) {

		GLuint locationCheck = glGetUniformLocation(shaderProgramID, uniformName[i].c_str());

		if (locationCheck == -1) {

			Logger::getInstance()->warningLog("SHADER_PROGRAM uniform not found");
			Logger::getInstance()->warningLog(uniformName[i].c_str());

			continue;

		}

		nameToLocation.emplace(uniformName[i], i);

		uniformLocation.push_back(locationCheck);

	}

	glUseProgram(0);

}

void Shader::updateUniformValue_INT(const int& passThrough, const char* uniformName) {

	int index = nameToLocation[uniformName];

	GLuint loc = uniformLocation[index];

	glUniform1i(loc, passThrough);

}

void Shader::updateUniformValue_FLOAT(const float& passThrough, const char* uniformName) {

	int index = nameToLocation[uniformName];

	GLuint loc = uniformLocation[index];

	glUniform1f(loc, passThrough);

}

void Shader::updateUniformValue_VEC3(const glm::vec3& passThrough, const char* uniformName) {

	int index = nameToLocation[uniformName];

	GLuint loc = uniformLocation[index];

	glUniform3f(loc, passThrough.x, passThrough.y, passThrough.z);

}

void Shader::updateUniformValue_VEC4(const glm::vec4& passThrough, const char* uniformName) {

	int index = nameToLocation[uniformName];

	GLuint loc = uniformLocation[index];

	glUniform4f(loc, passThrough.r, passThrough.g, passThrough.b, passThrough.a);

}

void Shader::updateUniformValue_MATRIX4(const glm::mat4& passThrough, const char* uniformName) {

	int index = nameToLocation[uniformName];

	GLuint loc = uniformLocation[index];

	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(passThrough));

}

GLuint Shader::getShaderProgramID() {

	return shaderProgramID;

}

bool Shader::getIsBinded() {

	return isBinded;

}