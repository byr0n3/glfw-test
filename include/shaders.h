#include "GL/glew.h"

namespace byrone::shaders {
	GLuint load(const std::string &vertex_path, const std::string &fragment_path);
}

std::string loadFromFile(const std::string &path);

void compile(GLuint id, const char *ptr);

GLuint compileProgram(GLuint vertexId, GLuint fragmentId);
