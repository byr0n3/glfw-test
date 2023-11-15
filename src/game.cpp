#include <include/game.h>
#include <include/sprite_renderer.h>
#include <include/resource_manager.h>

byrone::SpriteRenderer *spriteRenderer;

byrone::Game::Game(unsigned int width, unsigned int height) : state(byrone::GameState::GAME_ACTIVE),
															  width(width), height(height),
															  keys() {

}

byrone::Game::~Game() = default;

void byrone::Game::Init() const {
	auto w = static_cast<float>(this->width);
	auto h = static_cast<float>(this->height);

	byrone::Shader shader = byrone::ResourceManager::LoadShader("assets/shaders/sprite.vert",
																"assets/shaders/sprite.frag");

	glm::mat4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

	shader.Set("image", 0);
	shader.Set("projection", projection);

	spriteRenderer = new byrone::SpriteRenderer(shader);

	byrone::ResourceManager::LoadTexture("assets/textures/awesomeface.png", true);
}

void byrone::Game::Update(double deltaTime) {

}

void byrone::Game::ProcessInput(double deltaTime) {

}

void byrone::Game::Render() {
	auto position = glm::vec2(200.0f, 200.0f);
	auto size = glm::vec2(300.0f, 400.0f);
	auto color = glm::vec3(0.0f, 1.0f, 0.0f);

	auto texture = byrone::ResourceManager::GetTexture(1);
	spriteRenderer->Draw(texture, position, size, 45.0f, color);
}
