#ifndef BYRONE_BALL_OBJECT
#define BYRONE_BALL_OBJECT

#include <include/game_object.h>

namespace byrone {
	class BallObject : public GameObject {
	public:
		float Radius;
		bool Stuck;

		BallObject();

		BallObject(glm::vec2 position, Texture2D sprite, glm::vec2 velocity, float radius);

		glm::vec2 Move(double deltaTime, unsigned int window_width);

		void Reset(glm::vec2 position, glm::vec2 velocity);
	};
}

#endif
