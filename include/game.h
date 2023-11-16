#ifndef BYRONE_GAME
#define BYRONE_GAME

#include <vector>
#include <include/level.h>

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
	};
}

#endif
