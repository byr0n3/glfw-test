#ifndef BYRONE_MODEL
#define BYRONE_MODEL

#include <glm/glm.hpp>
#include "GL/glew.h"
#include <vector>

namespace byrone {
	class Model {
	public:
		unsigned int id;

		Model();

		Model(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals);

		GLuint vertexId;
		GLuint uvId;

		void Compile();

		GLsizei getVertexSize();

		GLsizei getUvSize();

	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		GLuint loadVertexBuffer();

		GLuint loadUvBuffer();
	};

	enum struct ModelType {
		OBJ,
	};
}

#endif
