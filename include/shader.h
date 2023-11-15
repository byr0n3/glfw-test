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
		Shader &Use();

		void Compile(const char *vertex, const char *fragment);

		void SetFloat(const char *name, float value, bool useShader = false);

		void SetInteger(const char *name, int value, bool useShader = false);

		void SetVector2f(const char *name, float x, float y, bool useShader = false);

		void SetVector2f(const char *name, const glm::vec2 &value, bool useShader = false);

		void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);

		void SetVector3f(const char *name, const glm::vec3 &value, bool useShader = false);

		void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);

		void SetVector4f(const char *name, const glm::vec4 &value, bool useShader = false);

		void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader = false);

	private:
		static void checkShaderErrors(unsigned int shaderId);

		static void checkProgramErrors(unsigned int programId);
	};
}

#endif
