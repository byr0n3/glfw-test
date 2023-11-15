#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <include/resource_manager.h>
#include <include/controls.h>
#include <sstream>

static double lastRenderTime;
static int frameCount;

void updatePerformanceMetrics(GLFWwindow *window, float deltaTime) {
	frameCount++;

	if (deltaTime >= 1.0) {
		auto frameTime = 1000.0 / (float) frameCount;
		auto fps = int(1000 / frameTime);

		std::stringstream temp;
		temp << fps << " FPS | " << frameTime << "ms";

		glfwSetWindowTitle(window, temp.str().c_str());
		frameCount = 0;
		lastRenderTime += 1.0;
	}
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

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW." << std::endl;
		glfwTerminate();
		return 1;
	}

	// Prevents missing state changes by only resetting them when `glfwGetKey` gets called
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable the Z-Buffer to only render triangles that we should render
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Don't render triangles that aren't seen by the camera
	glEnable(GL_CULL_FACE);

	// Create a blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Load the simple shaders
	byrone::Shader shader = byrone::ResourceManager::LoadShader("assets/shaders/simple_vertex_shader.vertexshader",
																"assets/shaders/simple_fragment_shader.fragmentshader");

	// Load our test texture
	byrone::Texture2D texture = byrone::ResourceManager::LoadTexture("assets/textures/uv.DDS",
																	 false,
																	 byrone::TextureType::DDS);

	// Load our model
	byrone::Model model = byrone::ResourceManager::LoadModel("assets/models/cube.obj", byrone::ModelType::OBJ);

	model.Compile();

	byrone::controls controls(window, glm::vec3(0, 0, 5), 0.1f, 0.005f);

	lastRenderTime = glfwGetTime();
	frameCount = 0;

	// Listen for the 'Escape' key for closing
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double currentTime = glfwGetTime();
		float deltaTime = (float) currentTime - (float) lastRenderTime;

		updatePerformanceMetrics(window, deltaTime);

		controls.update(deltaTime);

		// FoV, aspect ratio (4/3), near clipping plane, far clipping plane
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

		glm::vec3 position = controls.getPosition();
		glm::vec3 forward = controls.getForward();
		glm::vec3 up = controls.getUp();

		// Camera view
		glm::mat4 camera = glm::lookAt(position, position + forward, up);

		// Model view projection = multiplication of our 3 matrices
		glm::mat4 mvp = projection * camera;

		// Use our calculated rotation on the currently bound shader and enable the shader
		shader.SetMatrix4("mvp", mvp, true);

		// Bind our loaded texture and apply it on the shader
		texture.Bind();
		shader.SetInteger("textureSampler", 0);

		// Load the previously made buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, model.vertexId);
		glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void *) nullptr // offset
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, model.uvId);
		glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void *) nullptr
		);

		// Draw
		glDrawArrays(GL_TRIANGLES, 0, model.getVertexSize());
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap the buffer with the new one
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	byrone::ResourceManager::Clear();

	glfwTerminate();

	return 0;
}
