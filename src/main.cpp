#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <include/resource_manager.h>
#include <sstream>

// basic triangle shape
static const GLfloat cube_vertex[] = {
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
};

static GLfloat colors[12 * 3 * 3];

static double lastRenderTime;
static int frameCount;

void updatePerformanceMetrics(GLFWwindow *window) {
	double currentTime = glfwGetTime();
	double delta = currentTime - lastRenderTime;
	frameCount++;

	if (delta >= 1.0) {
		auto frameTime = 1000.0 / double(frameCount);
		auto fps = int(1000 / frameTime);

		std::stringstream temp;
		temp << fps << " FPS | " << frameTime << "ms";

		glfwSetWindowTitle(window, temp.str().c_str());
		frameCount = 0;
		lastRenderTime += 1.0;
	}
}

float rnd_color() {
	return (float) std::rand() / (float) RAND_MAX;
}

int main() {
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW." << std::endl;
		return 1;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	// Enable the Z-Buffer to only render triangles that we should renderit
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Create a blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Create a vertex
	GLuint vertexId;
	glGenVertexArrays(1, &vertexId);
	glBindVertexArray(vertexId);

	// Load the simple shaders
	byrone::Shader shader = byrone::ResourceManager::LoadShader("assets/shaders/simple_vertex_shader.vertexshader",
																"assets/shaders/simple_fragment_shader.fragmentshader");

	// FoV, aspect ratio (4/3), near clipping plane, far clipping plane
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera view
	glm::mat4 cameraView = glm::lookAt(
			// Camera position
			glm::vec3(4, 3, 3),
			// Camera rotation/angle
			glm::vec3(0, 0, 0),
			// And looks up (Vector3.up)
			glm::vec3(0, 1, 0)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	auto model = glm::mat4(1.0f);

	// Model view projection = multiplication of our 3 matrices
	glm::mat4 mvp = projection * cameraView * model;

	GLuint vertexBuffer;
	// Generate 1 buffer
	glGenBuffers(1, &vertexBuffer);
	// Use our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	// Give the triangle vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex), cube_vertex, GL_STATIC_DRAW);

	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

	lastRenderTime = glfwGetTime();
	frameCount = 0;

	// Listen for the 'Escape' key for closing
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		updatePerformanceMetrics(window);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < 12 * 3; i++) {
			colors[3 * i] = rnd_color();
			colors[3 * i + 1] = rnd_color();
			colors[3 * i + 2] = rnd_color();
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		// Use our calculated rotation on the currently bound shader and enable the shader
		shader.SetMatrix4("mvp", mvp, true);

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

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void *) nullptr
		);

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		glDisableVertexAttribArray(0);

		// Swap the buffer with the new one
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexId);

	byrone::ResourceManager::Clear();

	glfwTerminate();

	return 0;
}
