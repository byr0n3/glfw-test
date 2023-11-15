#include <include/shader.h>
#include <GL/glew.h>
#include <iostream>
#include <src/debug.h>

void byrone::Shader::Use() const {
	GL_CHECK(glUseProgram(this->id))
}

void byrone::Shader::End() const {
	GL_CHECK(glUseProgram(0))
}

void byrone::Shader::Compile(const char *vertex, const char *fragment) {
	// Compile the vertex shader
	unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
	GL_CHECK(glShaderSource(vertexId, 1, &vertex, nullptr))
	GL_CHECK(glCompileShader(vertexId))
	checkShaderErrors(vertexId);

	// Compile the fragment shader
	unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	GL_CHECK(glShaderSource(fragmentId, 1, &fragment, nullptr))
	GL_CHECK(glCompileShader(fragmentId))
	checkShaderErrors(fragmentId);

	// Create the shader program
	this->id = glCreateProgram();
	GL_CHECK(glAttachShader(this->id, vertexId))
	GL_CHECK(glAttachShader(this->id, fragmentId))

	GL_CHECK(glLinkProgram(this->id))
	checkProgramErrors(this->id);

	// Cleanup
	GL_CHECK(glDeleteShader(vertexId))
	GL_CHECK(glDeleteShader(fragmentId))
}

// @todo cache `glGetUniformLocation`

void byrone::Shader::Set(const char *name, float value) {
	GL_CHECK(glUniform1f(glGetUniformLocation(this->id, name), value))
}

void byrone::Shader::Set(const char *name, int value) {
	GL_CHECK(glUniform1i(glGetUniformLocation(this->id, name), value))
}

void byrone::Shader::Set(const char *name, float x, float y) {
	GL_CHECK(glUniform2f(glGetUniformLocation(this->id, name), x, y))
}

void byrone::Shader::Set(const char *name, const glm::vec2 &value) {
	GL_CHECK(glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y))
}

void byrone::Shader::Set(const char *name, float x, float y, float z) {
	GL_CHECK(glUniform3f(glGetUniformLocation(this->id, name), x, y, z))
}

void byrone::Shader::Set(const char *name, const glm::vec3 &value) {
	GL_CHECK(glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z))
}

void byrone::Shader::Set(const char *name, float x, float y, float z, float w) {
	GL_CHECK(glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w))
}

void byrone::Shader::Set(const char *name, const glm::vec4 &value) {
	GL_CHECK(glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w))
}

void byrone::Shader::Set(const char *name, const glm::mat4 &matrix) {
	GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix)))
}

void byrone::Shader::checkShaderErrors(unsigned int shaderId) {
	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (success) {
		return;
	}

	int logLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

	char error[logLength + 1];

	glGetShaderInfoLog(shaderId, logLength, nullptr, &error[0]);

	std::cout << "Error compiling shader:" << std::endl
			  << std::string(error) << std::endl;
}

void byrone::Shader::checkProgramErrors(unsigned int programId) {
	int success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (success) {
		return;
	}

	int logLength;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

	char error[logLength + 1];

	glGetProgramInfoLog(programId, logLength, nullptr, &error[0]);

	std::cout << "Error compiling program:" << std::endl
			  << std::string(error) << std::endl;
}
