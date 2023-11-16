#include <include/game.h>
#include <include/sprite_renderer.h>
#include <include/resource_manager.h>
#include "GLFW/glfw3.h"

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

byrone::SpriteRenderer *spriteRenderer;
byrone::GameObject *player;

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

	// Load player
	byrone::Texture2D playerTexture = byrone::ResourceManager::LoadTexture("paddle",
																		   "assets/textures/paddle.png",
																		   true);

	glm::vec2 playerPosition = glm::vec2(w / 2.0f - PLAYER_SIZE.x / 2.0f, h - PLAYER_SIZE.y);

	player = new byrone::GameObject(playerPosition, PLAYER_SIZE, playerTexture);

	// Load background
	ResourceManager::LoadTexture("background", "assets/textures/background.png", false);

	// Load textures
	byrone::ResourceManager::LoadTexture("block", "assets/textures/block.png", false);
	byrone::ResourceManager::LoadTexture("block_solid", "assets/textures/block_solid.png", false);

	// Load levels
	// @todo Lazy load and cache levels when needed
	byrone::Level level;
	level.Load("assets/levels/level.dat", this->Width, this->Height / 2);

	this->Levels.push_back(level);
	this->CurrentLevelIdx = 0;
}

void byrone::Game::Update(double deltaTime) {

}

void byrone::Game::ProcessInput(double deltaTime) {
	if (this->State != byrone::GameState::GAME_ACTIVE) {
		return;
	}

	float velocity = PLAYER_VELOCITY * static_cast<float>(deltaTime);

	if (this->Keys[GLFW_KEY_A] && player->Position.x >= 0.0f) {
		player->Position.x -= velocity;
	}

	if (this->Keys[GLFW_KEY_D] && player->Position.x <= (static_cast<float>(this->Width) - player->Size.x)) {
		player->Position.x += velocity;
	}
}

void byrone::Game::Render() {
	if (this->State == byrone::GameState::GAME_ACTIVE) {
		byrone::Texture2D bg = ResourceManager::GetTexture("background");

		spriteRenderer->Draw(bg, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

		player->Draw(*spriteRenderer);

		// @todo Don't do vector lookup every frame
		this->Levels[this->CurrentLevelIdx].Draw(*spriteRenderer);
	}
}
