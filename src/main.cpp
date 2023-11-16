#include <iostream>
// we MUST import/reference GLEW before importing GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <include/game.h>
#include <include/resource_manager.h>

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

// @todo Don't initialize here
byrone::Game game(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
	if (!glfwInit()) {
		std::cout << "Couldn't initialize GLFW" << std::endl;
		return 1;
	}

	// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Use the default(?) OpenGL profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Apple fix
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Disable resizing
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW." << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get the size of the frame buffer
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// Re-assign our game instance to match the frame buffer size
	game = byrone::Game(width, height);

	// Set callbacks
	glfwSetKeyCallback(window, key_callback);

	// Configure our viewport and OpenGL
	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game.Init();

	double deltaTime = 0.0f;
	double lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		// Calculate delta time
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		game.ProcessInput(deltaTime);

		game.Update(deltaTime);

		// Clear screen and render
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		game.Render();

		// Activate new buffer
		glfwSwapBuffers(window);
	}

	// Clear loaded resources
	byrone::ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	// close the window if we press the 'Escape' key
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	// invalid keys
	if (key < 0 || key >= 1024) {
		return;
	}

	if (action == GLFW_PRESS) {
		game.Keys[key] = true;
	} else if (action == GLFW_RELEASE) {
		game.Keys[key] = false;
	}
}
