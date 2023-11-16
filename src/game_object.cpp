#include <include/game_object.h>


byrone::GameObject::GameObject() : Position(0.0f, 0.0f), Size(1.0f, 1.0f),
								   Velocity(0.0f), Color(1.0f),
								   Rotation(0.0f), Sprite(),
								   IsSolid(false), Destroyed(false) {

}

byrone::GameObject::GameObject(glm::vec2 position,
							   glm::vec2 size,
							   Texture2D sprite,
							   float rotation,
							   glm::vec3 color,
							   glm::vec2 velocity) : Position(position), Size(size),
													 Velocity(velocity), Color(color),
													 Rotation(rotation), Sprite(sprite),
													 IsSolid(false), Destroyed(false) {

}

void byrone::GameObject::Draw(SpriteRenderer &renderer) {
	renderer.Draw(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
