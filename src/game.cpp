#include <include/game.h>
#include <include/sprite_renderer.h>
#include <include/resource_manager.h>

byrone::SpriteRenderer *spriteRenderer;

byrone::Game::Game(unsigned int width, unsigned int height) : State(byrone::GameState::GAME_ACTIVE),
															  Width(width), Height(height),
															  Keys() {

}

byrone::Game::~Game() = default;

void byrone::Game::Init() {
	auto w = static_cast<float>(this->Width);
	auto h = static_cast<float>(this->Height);

	glm::mat4 projection = glm::ortho(0.0f, w, h, 0.0f, -1.0f, 1.0f);

	// Load sprite shader
	byrone::Shader shader = byrone::ResourceManager::LoadShader("sprite",
																"assets/shaders/sprite.vert",
																"assets/shaders/sprite.frag");

	shader.Use();
	shader.Set("image", 0);
	shader.Set("projection", projection);

	spriteRenderer = new byrone::SpriteRenderer(shader);

	// Load textures
	byrone::ResourceManager::LoadTexture("block", "assets/textures/block.png", false);
	byrone::ResourceManager::LoadTexture("block_solid", "assets/textures/block_solid.png", false);

	// Load levels
	// @todo Lazy load levels when needed
	byrone::Level level;
	level.Load("assets/levels/level.dat", this->Width, this->Height / 2);

	this->Levels.push_back(level);
	this->CurrentLevelIdx = 0;
}

void byrone::Game::Update(double deltaTime) {

}

void byrone::Game::ProcessInput(double deltaTime) {

}

void byrone::Game::Render() {
	if (this->State == GameState::GAME_ACTIVE) {
		// @todo Draw background

		// @todo Don't do vector lookup every frame
		this->Levels[this->CurrentLevelIdx].Draw(*spriteRenderer);
	}
}
