#ifndef BYRONE_GAME_OBJECT
#define BYRONE_GAME_OBJECT

#include <glm/glm.hpp>
#include <include/texture2d.h>
#include <include/sprite_renderer.h>

namespace byrone {
	class GameObject {
	public:
		glm::vec2 Position, Size, Velocity;
		glm::vec3 Color;
		float Rotation;

		Texture2D Sprite;

		// @todo Flags
		bool IsSolid;
		bool Destroyed;

		GameObject();

		GameObject(glm::vec2 pos,
				   glm::vec2 size,
				   Texture2D sprite,
				   float rotation = 0.0f,
				   glm::vec3 color = glm::vec3(1.0f),
				   glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

		// Draw the sprite using the object's properties
		virtual void Draw(SpriteRenderer &renderer);
	};
}

#endif
