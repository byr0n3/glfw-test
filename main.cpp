#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <include/shaders.h>
#include <fstream>

// basic triangle shape
static const GLfloat triangle_vertex[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
};

int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW." << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

	// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Apparently fix for MacOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Use newer OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(1024, 768, "Game Test", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return 1;
	}

	// Initialize GLEW
	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW." << std::endl;
		glfwTerminate();
		return 1;
	}

	// Prevents missing state changes by only resetting them when `glfwGetKey` gets called
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Create a blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create a vertex
	GLuint vertexId;
	glGenVertexArrays(1, &vertexId);
	glBindVertexArray(vertexId);

	// Load the simple shaders
	GLuint programId = byrone::shaders::load("assets/shaders/simple_vertex_shader.vertexshader",
											 "assets/shaders/simple_fragment_shader.fragmentshader");

	GLuint vertexBuffer;
	// Generate 1 buffer
	glGenBuffers(1, &vertexBuffer);
	// Use our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// Give the triangle vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertex), triangle_vertex, GL_STATIC_DRAW);

	// Listen for the 'Escape' key for closing
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Load the previously made buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void *) nullptr // offset
		);

		glUseProgram(programId);

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		// Swap the buffer with the new one
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexId);
	glDeleteProgram(programId);

	glfwTerminate();

	return 0;
}
