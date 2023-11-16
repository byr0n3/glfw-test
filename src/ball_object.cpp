#include <include/ball_object.h>

byrone::BallObject::BallObject() : byrone::GameObject(), Radius(12.5f), Stuck(true) {}

byrone::BallObject::BallObject(glm::vec2 position,
							   byrone::Texture2D sprite,
							   glm::vec2 velocity,
							   float radius) {
	this->Position = position;
	this->Size = glm::vec2(radius * 2.0f, radius * 2.0f);
	this->Sprite = sprite;
	this->Rotation = 0.0f;
	this->Color = glm::vec3(1.0f);
	this->Velocity = velocity;

	this->Radius = radius;
	this->Stuck = true;
}

glm::vec2 byrone::BallObject::Move(double deltaTime, unsigned int window_width) {
	// Stuck on player paddle
	if (this->Stuck) {
		return this->Position;
	}

	auto width = static_cast<float>(window_width);

	this->Position += this->Velocity * static_cast<float>(deltaTime);

	// Check if outside window bounds. Reverse velocity and restore at correct position if so.
	if (this->Position.x <= 0.0f) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = 0.0f;
	} else if (this->Position.x + this->Size.x >= width) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = width - this->Size.x;
	}

	if (this->Position.y <= 0.0f) {
		this->Velocity.y = -this->Velocity.y;
		this->Position.y = 0.0f;
	}

	return this->Position;
}

// Place the ball at the specified position with the specified velocity and make it stuck
void byrone::BallObject::Reset(glm::vec2 position, glm::vec2 velocity) {
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}
