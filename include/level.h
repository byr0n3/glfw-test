#ifndef BYRONE_LEVEL
#define BYRONE_LEVEL

#include <vector>
#include <include/game_object.h>
#include <include/sprite_renderer.h>

namespace byrone {
	// LEVEL FILE DATA
	// 0: air
	// 1: un-destroyable brick
	// > 1: a destroyable brick with a color equal to the number

	class Level {
	public:
		std::vector<GameObject> Bricks;

		Level();

		// @todo Move to ResourceManager?
		// Load the level from a file
		void Load(const char *file, unsigned int width, unsigned int height);

		// Draw the level
		void Draw(SpriteRenderer &renderer);

		// Checks if there's any bricks left
		bool IsCompleted();

	private:
		// Initialize the level
		void init(std::vector<std::vector<unsigned int>> tiles, unsigned int width, unsigned int height);
	};
}

#endif
