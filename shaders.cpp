#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "include/shaders.h"

GLuint byrone::shaders::load(const std::string &vertex_path, const std::string &fragment_path) {
	// Create the shaders we need
	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the shader contents from the specified path
	std::string vertexShader = loadFromFile(vertex_path);
	std::string fragmentShader = loadFromFile(fragment_path);

	compile(vertexId, vertexShader.c_str());
	compile(fragmentId, fragmentShader.c_str());

	GLuint programId = compileProgram(vertexId, fragmentId);

	// Cleanup

	glDetachShader(programId, vertexId);
	glDetachShader(programId, fragmentId);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return programId;
}

std::string loadFromFile(const std::string &path) {
	std::string result;
	std::ifstream stream("assets/shaders/" + path, std::ios::in);

	if (!stream.is_open()) {
		std::cout << "Couldn't open '" << path << "'. Make sure the files exists." << std::endl;

		return result;
	}

	std::stringstream strStream;
	strStream << stream.rdbuf();

	result = strStream.str();

	stream.close();

	return result;
}

void compile(GLuint id, const char *ptr) {
	GLint result = GL_FALSE;
	int logLength;

	// Compile the shader
	glShaderSource(id, 1, &ptr, nullptr);
	glCompileShader(id);

	// Validate the shader
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> error(logLength + 1);

		glGetShaderInfoLog(id, logLength, nullptr, &error[0]);

		std::cout << error[0] << std::endl;
	}
}

GLuint compileProgram(GLuint vertexId, GLuint fragmentId) {
	GLint result = GL_FALSE;
	int logLength;

	// Create the program & link the shaders
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);
	glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> error(logLength + 1);

		glGetProgramInfoLog(programId, logLength, nullptr, &error[0]);

		std::cout << error[0] << std::endl;
	}

	return programId;
}
