#ifndef BYRONE_SHADER_H
#define BYRONE_SHADER_H

#include <glm/glm.hpp>
// used in shader.cpp
#include <glm/gtc/type_ptr.hpp>

namespace byrone {
	class Shader {
	public:
		unsigned int id;

		// Activate the shader
		void Use() const;

		void End() const;

		void Compile(const char *vertex, const char *fragment);

		void Set(const char *name, float value);

		void Set(const char *name, int value);

		void Set(const char *name, float x, float y);

		void Set(const char *name, const glm::vec2 &value);

		void Set(const char *name, float x, float y, float z);

		void Set(const char *name, const glm::vec3 &value);

		void Set(const char *name, float x, float y, float z, float w);

		void Set(const char *name, const glm::vec4 &value);

		void Set(const char *name, const glm::mat4 &matrix);

	private:
		static void checkShaderErrors(unsigned int shaderId);

		static void checkProgramErrors(unsigned int programId);
	};
}

#endif
