#include <include/sprite_renderer.h>

byrone::SpriteRenderer::SpriteRenderer(Shader &shader) {
	this->shader = shader;
	this->initialize();
}

byrone::SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->vao);
}

void byrone::SpriteRenderer::initialize() {
	unsigned int vbo;

	GLfloat vertices[] = {
			// Pos      // Tex
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
	};

	// Generate a buffer for the sprite vertices on the GPU
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &vbo);

	// Send the sprite vertices to the buffer
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Enable the buffer
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Because multiplying matrices occurs from right to left, we apply transformations in the following order:
// translate (move), rotate, scale
void byrone::SpriteRenderer::Draw(Texture2D &texture,
                                  glm::vec2 position,
                                  glm::vec2 size,
                                  float rotation,
                                  glm::vec3 color) {
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);

	// Move
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// Move by half our size to rotate around the center of the sprite instead of the top left
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	// Apply the actual rotation
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	// Move back to the previous position
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	// Scale
	model = glm::scale(model, glm::vec3(size, 1.0f));

	// Give the sprite rendering shader our transform and color data
	this->shader.Set("model", model);
	this->shader.Set("spriteColor", color);

	// Bind our texture
	texture.Bind();

	// Draw our matrices
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, 4);
	glBindVertexArray(0);

	shader.End();
	texture.Unbind();
}