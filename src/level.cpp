#include <include/level.h>
#include <include/resource_manager.h>
#include <iostream>
#include <sstream>
#include <fstream>

byrone::Level::Level() {
}

void byrone::Level::Load(const char *filePath, unsigned int width, unsigned int height) {
	this->Bricks.clear();

	unsigned int tileCode;
	Level level;
	std::string line;
	std::ifstream stream(filePath);
	std::vector<std::vector<unsigned int>> tiles;

	if (!stream) {
		std::cout << "Unable to open level file:" << std::endl
				  << filePath << std::endl;

		return;
	}

	while (std::getline(stream, line)) {
		std::istringstream sstream(line);
		std::vector<unsigned int> row;

		while (sstream >> tileCode) {
			row.push_back(tileCode);
		}

		tiles.push_back(row);
	}

	this->init(tiles, width, height);
}

void byrone::Level::Draw(byrone::SpriteRenderer &renderer) {
	for (GameObject &tile: this->Bricks) {
		if (!tile.Destroyed) {
			tile.Draw(renderer);
		}
	}
}

bool byrone::Level::IsCompleted() {
	// @todo std::ranges::all_of() faster?
	for (const GameObject &tile: this->Bricks) {
		if (!tile.IsSolid && !tile.Destroyed) {
			return false;
		}
	}

	return true;
}

void byrone::Level::init(std::vector<std::vector<unsigned int>> tiles, unsigned int width, unsigned int height) {
	if (tiles.empty()) {
		return;
	}

	unsigned int tileHeight = tiles.size();
	unsigned int tileWidth = tiles[0].size();

	float unitWidth = static_cast<float>(width) / static_cast<float>(tileWidth);
	float unitHeight = static_cast<float>(height) / static_cast<float>(tileHeight);

	glm::vec3 colorSolid(0.8f, 0.8f, 0.7f);

	Texture2D blockSolid = byrone::ResourceManager::GetTexture("block_solid");
	Texture2D block = byrone::ResourceManager::GetTexture("block");

	// loop over all tiles on the X and Y axis
	for (unsigned int y = 0; y < tileHeight; ++y) {
		for (unsigned int x = 0; x < tileWidth; ++x) {
			unsigned int tile = tiles[y][x];

			// Air block, skip
			if (tile == 0) {
				continue;
			}

			// @todo Fix 0,0,0 doesn't point to the top-left corner
			glm::vec2 position(unitWidth * static_cast<float>(x), unitHeight * static_cast<float>(y));
			glm::vec2 size(unitWidth, unitHeight);

			// Solid brick
			if (tile == 1) {
				GameObject obj(position, size, blockSolid, 0.0f, colorSolid);
				obj.IsSolid = true;

				this->Bricks.push_back(obj);

				continue;
			}

			// Default to white
			glm::vec3 color(1.0f, 1.0f, 1.0f);

			// @todo Refactor
			switch (tile) {
				case 2:
					color = glm::vec3(0.2f, 0.6f, 1.0f);
					break;

				case 3:
					color = glm::vec3(0.0f, 0.7f, 0.0f);
					break;

				case 4:
					color = glm::vec3(0.8f, 0.8f, 0.4f);
					break;

				case 5:
					color = glm::vec3(1.0f, 0.5f, 0.0f);
					break;

				default:
					break;
			}

			GameObject obj(position, size, block, 0.0f, color);
			this->Bricks.push_back(obj);
		}
	}
}
