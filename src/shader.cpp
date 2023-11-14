#include "include/shader.h"
#include "GL/glew.h"
#include <iostream>
#include <vector>

byrone::Shader &byrone::Shader::Use() {
	glUseProgram(this->id);

	return *this;
}

void byrone::Shader::Compile(const char *vertex, const char *fragment) {
	unsigned int vertexId, fragmentId;

	// Compile the vertex shader
	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vertex, nullptr);
	glCompileShader(vertexId);
	checkShaderErrors(vertexId);

	// Compile the fragment shader
	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
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
	glDetachShader(this->id, vertexId);
	glDetachShader(this->id, fragmentId);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
}

void byrone::Shader::SetFloat(const char *name, float value, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform1f(glGetUniformLocation(this->id, name), value);
}

void byrone::Shader::SetInteger(const char *name, int value, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform1i(glGetUniformLocation(this->id, name), value);
}

void byrone::Shader::SetVector2f(const char *name, float x, float y, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform2f(glGetUniformLocation(this->id, name), x, y);
}

void byrone::Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}

void byrone::Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void byrone::Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}

void byrone::Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void byrone::Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}

void byrone::Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader) {
	if (useShader) {
		this->Use();
	}

	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(matrix));
}

void byrone::Shader::checkShaderErrors(unsigned int shaderId) {
	int success;

	glGetProgramiv(shaderId, GL_LINK_STATUS, &success);

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

	glGetShaderiv(programId, GL_COMPILE_STATUS, &success);

	if (success) {
		return;
	}

	int logLength;
	glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &logLength);

	std::vector<char> error(logLength + 1);

	glGetProgramInfoLog(programId, logLength, nullptr, &error[0]);

	std::cout << "Error compiling program:" << std::endl
			  << std::string(error.begin(), error.end()) << std::endl;
}
