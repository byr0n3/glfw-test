#include <include/game.h>
#include <include/ball_object.h>
#include <include/sprite_renderer.h>
#include <include/resource_manager.h>
#include "GLFW/glfw3.h"

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
// Initial velocity of the player paddle
const float PLAYER_VELOCITY(500.0f);

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 18.75f;

byrone::SpriteRenderer *spriteRenderer;
byrone::GameObject *player;
byrone::BallObject *ball;

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

	// Load ball
	byrone::Texture2D ballTexture = byrone::ResourceManager::LoadTexture("ball",
																		 "assets/textures/ball.png",
																		 true);

	glm::vec2 ballPosition = playerPosition + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

	ball = new byrone::BallObject(ballPosition, ballTexture, INITIAL_BALL_VELOCITY, BALL_RADIUS);

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
	ball->Move(deltaTime, this->Width);
}

void byrone::Game::ProcessInput(double deltaTime) {
	if (this->State != byrone::GameState::GAME_ACTIVE) {
		return;
	}

	float velocity = PLAYER_VELOCITY * static_cast<float>(deltaTime);

	if (this->Keys[GLFW_KEY_A] && player->Position.x >= 0.0f) {
		player->Position.x -= velocity;

		if (ball->Stuck) {
			ball->Position.x -= velocity;
		}
	}

	if (this->Keys[GLFW_KEY_D] && player->Position.x <= (static_cast<float>(this->Width) - player->Size.x)) {
		player->Position.x += velocity;

		if (ball->Stuck) {
			ball->Position.x += velocity;
		}
	}

	if (this->Keys[GLFW_KEY_SPACE]) {
		ball->Stuck = false;
	}
}

void byrone::Game::Render() {
	if (this->State == byrone::GameState::GAME_ACTIVE) {
		byrone::Texture2D bg = ResourceManager::GetTexture("background");

		spriteRenderer->Draw(bg, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);

		player->Draw(*spriteRenderer);

		ball->Draw(*spriteRenderer);

		// @todo Don't do vector lookup every frame
		this->Levels[this->CurrentLevelIdx].Draw(*spriteRenderer);
	}
}
