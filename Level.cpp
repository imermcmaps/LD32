#include "Level.hpp"
#include "LD32.hpp"
#include <Engine/Button.hpp>
#include <iostream>
Level::Level(LD32* game): Scene(game), m_slayer(nullptr), m_initialized(false), m_paused(false) {
	
}

Level::~Level() {
}
void Level::update(sf::Time interval) {
	auto paused = m_ui->GetChildByID("paused");
	if (!m_initialized) {
		m_initialized=true;
		auto button = static_cast<engine::Button*>(paused->GetChildByID("button"));
		button->OnClick = [this](engine::Button*, sf::Vector2f){
			m_paused = false;
		};
	}
	if (!m_game->IsFocus()) {
		m_paused = true;
		paused->SetActive(true);
	} 
	if (m_paused) {
		m_ui->update(interval);
	} else {
		paused->SetActive(false);
		engine::Scene::update(interval);
	}
}
