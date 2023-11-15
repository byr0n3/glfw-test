#include <include/game.h>

byrone::Game::Game(unsigned int width, unsigned int height) : state(byrone::GameState::GAME_ACTIVE),
															  width(width), height(height),
															  keys() {

}

byrone::Game::~Game() {
}

void byrone::Game::Init() {

}

void byrone::Game::Update(double deltaTime) {

}

void byrone::Game::ProcessInput(double deltaTime) {

}

void byrone::Game::Render() {

}
