#include "Level.hpp"
#include "LD32.hpp"
#include "Slayer.hpp"
#include "Engine/Factory.hpp"
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
void Level::Respawn() {
	if (!m_slayer) return;
	std::string f = m_slayer->GetFilename();
	m_slayer->Delete();
	auto m_slayer = engine::Factory::CreateChildFromFile(f, this);
	m_slayer->SetPosition(m_respawnPoint.x, m_respawnPoint.y);
}
bool Level::initialize(Json::Value& root) {
	if (!engine::Scene::initialize(root)) return false;
	m_respawnPoint = sf::Vector2f(root["respawn"].get(0u, 100.0f).asFloat(), root["respawn"].get(1u, 100.0f).asFloat()); 
}
