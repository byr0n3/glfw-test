#include <include/game.h>
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

static void bounce(byrone::BallObject &ball, const byrone::GameObject &obj) {
	auto pos2 = ball.Position + ball.Radius;
	auto nearest = glm::max(obj.Position, glm::min(pos2, obj.Position + obj.Size));
	auto distance = pos2 - nearest;
	auto normalizedDistance = glm::normalize(distance);

	// if circle is moving toward the rect
	if (glm::dot(ball.Velocity, distance) < 0.0f) {
		auto tangent_vel = glm::dot(normalizedDistance, ball.Velocity);
		ball.Velocity = ball.Velocity - (tangent_vel * 2.0f);
	}

	auto penetrationDepth = ball.Radius - glm::length(distance);
	auto penetrationVector = normalizedDistance * penetrationDepth;
	ball.Position = ball.Position - penetrationVector;
}

byrone::Game::Game(unsigned int width, unsigned int height) : State(byrone::GameState::GAME_ACTIVE),
															  Width(width), Height(height),
															  Keys(), CurrentLevelIdx(0) {

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

	if (!ball->Stuck) {
		this->CheckCollisions();
	}
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

void byrone::Game::CheckCollisions() {
	glm::vec2 difference;

	// @todo Don't do vector lookup every frame
	// Check collision with the brick blocks of the current level
	for (byrone::GameObject &obj: this->Levels[this->CurrentLevelIdx].Bricks) {
		if (obj.Destroyed) {
			continue;
		}

		bool colliding = byrone::Game::CheckCollision(*ball, obj, difference);

		if (!colliding) {
			continue;
		}

		if (!obj.IsSolid) {
			obj.Destroyed = true;
		}

		bounce(*ball, obj);
	}

	// Check collision with the player paddle
	bool playerCollision = byrone::Game::CheckCollision(*ball, *player, difference);

	if (!playerCollision) {
		return;
	}

	bounce(*ball, *player);
}

// AABB
bool byrone::Game::CheckCollision(GameObject &one, GameObject &two) {
	// Collides on the X-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
					  two.Position.x + two.Size.x >= one.Position.x;

	// Collides on the Y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
					  two.Position.y + two.Size.y >= one.Position.y;

	return collisionX && collisionY;
}

// AABB Circle
bool byrone::Game::CheckCollision(const byrone::BallObject &ball,
								  const byrone::GameObject &obj,
								  glm::vec2 &difference) {
	glm::vec2 center(ball.Position + ball.Radius);

	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(obj.Size.x / 2.0f, obj.Size.y / 2.0f);
	glm::vec2 aabb_center(
			obj.Position.x + aabb_half_extents.x,
			obj.Position.y + aabb_half_extents.y
	);

	difference = center - aabb_center;

	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	return glm::dot(difference, difference) <= ball.Radius * ball.Radius;
}
