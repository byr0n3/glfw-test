#include "include/controls.h"

byrone::controls::controls(GLFWwindow *window, glm::vec3 position, float moveSpeed, float mouseSpeed) {
	this->window = window;
	this->position = position;
	this->moveSpeed = moveSpeed;
	this->mouseSpeed = mouseSpeed;

	this->horizontalAngle = 3.14f;
	this->verticalAngle = 0.0f;
}

void byrone::controls::update(float deltaTime) {
	this->handleMouse(deltaTime);

	this->handleKeys(deltaTime);
}

void byrone::controls::handleMouse(float deltaTime) {
	double xPos, yPos;
	glfwGetCursorPos(this->window, &xPos, &yPos);

	// always get the window size every update, in case it resized
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);

	// put cursor back to the center
	glfwSetCursorPos(this->window, (float) width / 2, (float) height / 2);

	this->horizontalAngle += this->mouseSpeed * deltaTime * float(width / 2 - xPos);
	this->verticalAngle += this->mouseSpeed * deltaTime * float(height / 2 - yPos);

	if (this->verticalAngle > byrone::controls::clampY) {
		this->verticalAngle = byrone::controls::clampY;
	} else if (this->verticalAngle < -byrone::controls::clampY) {
		this->verticalAngle = -byrone::controls::clampY;
	}
}

void byrone::controls::handleKeys(float deltaTime) {
	glm::vec3 forward = this->getForward();
	glm::vec3 right = this->getRight();

	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
		position += forward * deltaTime * this->moveSpeed;
	}

	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * this->moveSpeed;
	}

	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= forward * deltaTime * this->moveSpeed;
	}

	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * this->moveSpeed;
	}
}

glm::vec3 byrone::controls::getPosition() const {
	return this->position;
}

glm::vec3 byrone::controls::getForward() const {
	return {
			cos(this->verticalAngle) * sin(this->horizontalAngle),
			sin(this->verticalAngle),
			cos(this->verticalAngle) * cos(this->horizontalAngle)
	};;
}

glm::vec3 byrone::controls::getRight() const {
	// 3.14f = pi but not quite
	return {
			sin(this->horizontalAngle - 3.14f / 2.0f),
			0,
			cos(this->horizontalAngle - 3.14f / 2.0f)
	};
}

glm::vec3 byrone::controls::getUp() const {
	return glm::cross(this->getRight(), this->getForward());
}
