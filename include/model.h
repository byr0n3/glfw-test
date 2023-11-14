#ifndef BYRONE_MODEL
#define BYRONE_MODEL

#include <glm/glm.hpp>
#include "GL/glew.h"
#include <vector>

namespace byrone {
	class Model {
	public:
		Model(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals);

		std::vector<glm::vec3> getVertices();

		std::vector<glm::vec2> getUvs();

		std::vector<glm::vec3> getNormals();

		GLuint loadVertexBuffer();

		GLuint loadUvBuffer();

	private:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
	};

	enum struct ModelType {
		OBJ,
	};
}

#endif
