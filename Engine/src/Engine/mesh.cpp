#include "mesh.h"

Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, const std::vector<glm::vec3>& normals) {

	initialised = false;

	vao = 0;
	vbo = 0;
	ebo = 0;

	this->vertices = vertices;
	this->indices = indices;
	this->normals = normals;

}

Mesh::~Mesh() {

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);

}

void Mesh::addToBuffer(const std::vector<glm::vec4>& colour) {

	if (initialised) { return; }

	this->colour = colour;

}

void Mesh::addToBuffer(const std::vector<glm::vec2>& uv) {

	if (initialised) { return; }

	this->uv = uv;

}

void Mesh::init() {
	
	if (initialised) { return; }

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (vbo == 0) {

		int verticesSize = vertices.size() * sizeof(vertices);
		int normalsSize = normals.size() * sizeof(normals);
		int colourSize = colour.size() * sizeof(colour);
		int uvSize = uv.size() * sizeof(uv);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, verticesSize + normalsSize + colourSize + uvSize, vertices.data(), GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, vertices.data());
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, normals.data());
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, colourSize, colour.data());
		glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize + colourSize, uvSize, uv.data());

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, (void*)verticesSize);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, (void*)(verticesSize + normalsSize));
		glVertexAttribPointer(3, 2, GL_FLOAT, false, 0, (void*)(verticesSize + normalsSize + colourSize));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

	}

	if (ebo == 0) {

		int indicesSize = indices.size() * sizeof(indices);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);

	}

	initialised = true;

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

unsigned int Mesh::getVAO() {
	return vao;
}

unsigned int Mesh::getVBO() {
	return vbo;
}

unsigned int Mesh::getEBO() {
	return ebo;
}
