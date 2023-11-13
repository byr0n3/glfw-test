#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "include/shaders.h"

GLuint byrone::shaders::load(const char *vertex_path, const char *fragment_path) {
	// Create the shaders we need
	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the vertex shader from the specified path
	std::string vertexShader;
	std::ifstream vertexStream(vertex_path, std::ios::in);

	if (!vertexStream.is_open()) {
		std::cout << "Couldn't open '" << vertex_path << "'. Make sure the files exists." << std::endl;
		return 0;
	}

	std::stringstream vertextStrStream;
	vertextStrStream << vertexStream.rdbuf();
	vertexShader = vertextStrStream.str();
	vertexStream.close();

	// Read the fragment shader from the specified path
	std::string fragmentShader;
	std::ifstream fragmentStream(fragment_path, std::ios::in);

	if (fragmentStream.is_open()) {
		std::stringstream fragmentStrStream;
		fragmentStrStream << fragmentStream.rdbuf();
		fragmentShader = fragmentStrStream.str();
		fragmentStream.close();
	}

	GLint result = GL_FALSE;
	int logLength;

	// Compile the vertex shader

	std::cout << "Compiling shader: " << vertex_path << std::endl;

	auto vertexPointer = vertexShader.c_str();
	glShaderSource(vertexId, 1, &vertexPointer, nullptr);
	glCompileShader(vertexId);

	// Validate the vertex shader
	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexId, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> vertexError(logLength + 1);

		glGetShaderInfoLog(vertexId, logLength, nullptr, &vertexError[0]);

		std::cout << vertexError[0] << std::endl;
	}

	// Compile the fragment shader

	std::cout << "Compiling shader: " << fragment_path << std::endl;

	auto fragmentPointer = fragmentShader.c_str();
	glShaderSource(fragmentId, 1, &fragmentPointer, nullptr);
	glCompileShader(fragmentId);

	// Validate the fragment shader
	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentId, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> fragmentError(logLength + 1);

		glGetShaderInfoLog(fragmentId, logLength, nullptr, &fragmentError[0]);

		std::cout << fragmentError[0] << std::endl;
	}

	// Link the shaders to the program

	std::cout << "Linking program" << std::endl;

	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexId);
	glAttachShader(programId, fragmentId);
	glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 0) {
		std::vector<char> programError(logLength + 1);

		glGetProgramInfoLog(programId, logLength, nullptr, &programError[0]);

		std::cout << programError[0] << std::endl;
	}

	// Cleanup

	glDetachShader(programId, vertexId);
	glDetachShader(programId, fragmentId);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	return programId;
}
