#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <unordered_map>
#include <string>

class Shader {

private:

	friend class Renderer;

private:

	GLuint shaderProgramID;

	std::vector<GLuint> uniformLocation;
	std::unordered_map<std::string, int> nameToLocation;

	bool isBinded;

public:

	Shader(const char* fragmentPath, const char* vertexPath);
	~Shader();

	void init(const std::vector<std::string>& uniformName, const int& totalUniformVariables);

	void updateUniformValue_INT(const int& passThrough, const char* uniformName);
	void updateUniformValue_FLOAT(const float& passThrough, const char* uniformName);
	void updateUniformValue_VEC3(const glm::vec3& passThrough, const char* uniformName);
	void updateUniformValue_VEC4(const glm::vec4& passThrough, const char* uniformName);
	void updateUniformValue_MATRIX4(const glm::mat4& passThrough, const char* uniformName);

	GLuint getShaderProgramID();
	bool getIsBinded();

};