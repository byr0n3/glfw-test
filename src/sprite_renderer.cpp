#include <include/sprite_renderer.h>
#include <src/debug.h>

static const GLfloat vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
};

byrone::SpriteRenderer::SpriteRenderer(Shader &shader) {
	this->shader = shader;
	this->initialize();
}

byrone::SpriteRenderer::~SpriteRenderer() {
	glDeleteVertexArrays(1, &this->vao);
}

void byrone::SpriteRenderer::initialize() {
	unsigned int vbo;

	GL_CHECK(glGenVertexArrays(1, &this->vao))
	GL_CHECK(glGenBuffers(1, &vbo))

	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo))
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW))

	GL_CHECK(glBindVertexArray(this->vao))
	GL_CHECK(glEnableVertexAttribArray(0))
	GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr))
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0))
	GL_CHECK(glBindVertexArray(0))
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
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.Set("model", model);
	this->shader.Set("spriteColor", color);

	GL_CHECK(glActiveTexture(GL_TEXTURE0))
	texture.Bind();

	GL_CHECK(glBindVertexArray(this->vao))
	GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6))
	GL_CHECK(glBindVertexArray(0))
}
