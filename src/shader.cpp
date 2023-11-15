#include <include/shader.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>

void byrone::Shader::Use() const {
	glUseProgram(this->id);
}

void byrone::Shader::End() const {
	glUseProgram(0);
}

void byrone::Shader::Compile(const char *vertex, const char *fragment) {
	// Compile the vertex shader
	unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vertex, nullptr);
	glCompileShader(vertexId);
	checkShaderErrors(vertexId);

	// Compile the fragment shader
	unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentId, 1, &fragment, nullptr);
	glCompileShader(fragmentId);
	checkShaderErrors(fragmentId);

	// Create the shader program
	this->id = glCreateProgram();
	glAttachShader(this->id, vertexId);
	glAttachShader(this->id, fragmentId);

	glLinkProgram(this->id);
	checkProgramErrors(this->id);

	// Cleanup
	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

void byrone::Shader::Set(const char *name, float value) {
	glUniform1f(glGetUniformLocation(this->id, name), value);
}

void byrone::Shader::Set(const char *name, int value) {
	glUniform1i(glGetUniformLocation(this->id, name), value);
}

void byrone::Shader::Set(const char *name, float x, float y) {
	glUniform2f(glGetUniformLocation(this->id, name), x, y);
}

void byrone::Shader::Set(const char *name, const glm::vec2 &value) {
	glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}

void byrone::Shader::Set(const char *name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void byrone::Shader::Set(const char *name, const glm::vec3 &value) {
	glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}

void byrone::Shader::Set(const char *name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void byrone::Shader::Set(const char *name, const glm::vec4 &value) {
	glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}

void byrone::Shader::Set(const char *name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}

void byrone::Shader::checkShaderErrors(unsigned int shaderId) {
	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (success) {
		return;
	}

	int logLength;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

	std::vector<char> error(logLength + 1);

	glGetShaderInfoLog(shaderId, logLength, nullptr, &error[0]);

	std::cout << "Error compiling shader:" << std::endl
			  << std::string(error.begin(), error.end()) << std::endl;
}

void byrone::Shader::checkProgramErrors(unsigned int programId) {
	int success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (success) {
		return;
	}

	int logLength;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

	std::vector<char> error(logLength + 1);

	glGetProgramInfoLog(programId, logLength, nullptr, &error[0]);

	std::cout << "Error compiling program:" << std::endl
			  << std::string(error.begin(), error.end()) << std::endl;
}
