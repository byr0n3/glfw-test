#ifndef BYRONE_GAME
#define BYRONE_GAME

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

		GameState state;

		bool keys[1024];

		unsigned int width, height;

		void Init() const;

		void ProcessInput(double deltaTime);

		void Update(double deltaTime);

		void Render();
	};
}

#endif
