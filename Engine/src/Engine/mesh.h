#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

class Mesh {

private:

	bool initialised;

	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec3> normals;

	std::vector<glm::vec4> colour;
	std::vector<glm::vec2> uv;

public:

	Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, const std::vector<glm::vec3>& normals);
	~Mesh();

	void addToBuffer(const std::vector<glm::vec4>& colour);
	void addToBuffer(const std::vector<glm::vec2>& uv);

	void init();

	unsigned int getVAO();
	unsigned int getVBO();
	unsigned int getEBO();

};