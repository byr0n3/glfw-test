#include <utility>

#include "include/model.h"

byrone::Model::Model(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals) {
	this->vertices = std::move(vertices);
	this->uvs = std::move(uvs);
	this->normals = std::move(normals);
}

std::vector<glm::vec3> byrone::Model::getVertices() {
	return this->vertices;
}

std::vector<glm::vec2> byrone::Model::getUvs() {
	return this->uvs;
}

std::vector<glm::vec3> byrone::Model::getNormals() {
	return this->normals;
}

GLuint byrone::Model::loadVertexBuffer() {
	auto size = (GLsizeiptr) (this->vertices.size() * sizeof(glm::vec3));

	GLuint buffer;
	// Generate 1 buffer
	glGenBuffers(1, &buffer);
	// Use our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// Give the triangle vertices
	glBufferData(GL_ARRAY_BUFFER, size, &this->vertices[0], GL_STATIC_DRAW);

	return buffer;
}

GLuint byrone::Model::loadUvBuffer() {
	auto size = (GLsizeiptr) (this->uvs.size() * sizeof(glm::vec2));

	GLuint buffer;
	// Generate 1 buffer
	glGenBuffers(1, &buffer);
	// Use our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// Give the triangle vertices
	glBufferData(GL_ARRAY_BUFFER, size, &this->uvs[0], GL_STATIC_DRAW);

	return buffer;
}
