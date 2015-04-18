#include "Level.hpp"
#include "LD32.hpp"
Level::Level(LD32* game): Scene(game), m_slayer(nullptr) {
}

Level::~Level() {
}

