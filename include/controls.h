#ifndef BYRONE_CONTROLS
#define BYRONE_CONTROLS

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace byrone {
	class controls {
	public:
		controls(GLFWwindow *window, glm::vec3 position, float moveSpeed, float mouseSpeed);

		void update(float deltaTime);

		glm::vec3 getPosition() const;

		glm::vec3 getForward() const;

		glm::vec3 getRight() const;

		glm::vec3 getUp() const;

	private:
		const float clampY = 90.0f;

		GLFWwindow *window;

		glm::vec3 position;
		float horizontalAngle;
		float verticalAngle;
		float moveSpeed;
		float mouseSpeed;

		void handleMouse(float deltaTime);

		void handleKeys(float deltaTime);
	};
}

#endif
