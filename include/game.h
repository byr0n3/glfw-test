#ifndef BYRONE_GAME
#define BYRONE_GAME

#include <vector>
#include <include/level.h>
#include <include/game_object.h>
#include <include/ball_object.h>

namespace byrone {
	enum struct GameState {
		GAME_MENU,
		GAME_ACTIVE,
		GAME_WIN,
	};

	class Game {
	public:
		Game(unsigned int width, unsigned int height);

		~Game();

		GameState State;

		bool Keys[1024];

		unsigned int Width, Height;

		std::vector<Level> Levels;
		unsigned int CurrentLevelIdx;

		void Init();

		void ProcessInput(double deltaTime);

		void Update(double deltaTime);

		void Render();

	private:
		void CheckCollisions();

		static bool CheckCollision(GameObject &one, GameObject &two);

		static bool CheckCollision(const byrone::BallObject &ball,
								   const byrone::GameObject &obj,
								   glm::vec2 &difference);
	};
}

#endif
