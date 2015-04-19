#include "Level.hpp"
#include "LD32.hpp"
#include "Slayer.hpp"
#include "Engine/Factory.hpp"
#include <Engine/Button.hpp>
#include <iostream>
#include <sstream>
#include <Engine/Text.hpp>
Level::Level(LD32* game): Scene(game), m_slayer(nullptr), m_initialized(false), 
		m_paused(false), m_score(0), m_enemies(0) {
	
}

Level::~Level() {
	
}
void Level::update(sf::Time interval) {
	auto paused = m_ui->GetChildByID("paused");
	// meh
	if (!m_initialized) {
		m_initialized=true;
		ChangeScore(static_cast<LD32*>(m_game)->GetScore());
		auto button = static_cast<engine::Button*>(paused->GetChildByID("button"));
		button->OnClick = [this](engine::Button*, sf::Vector2f){
			m_paused = false;
		};
	}
	if (m_enemies == 0) {
		m_ui->GetChildByID("next")->SetActive(true);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
			m_game->SwitchScene(engine::Factory::create<Level>(m_next, static_cast<LD32*>(m_game)));
		}
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
void Level::ChangeScore(int amount) {
	m_score+=amount;
	auto text = static_cast<engine::Text*>(m_ui->GetChildByID("score"));
	if (text) {
		std::ostringstream ss;
		ss << "Score: " << m_score;
		text->SetText(ss.str());
		if (m_score < 0){
			text->SetColor(sf::Color(211, 47, 44));
		} else {
			text->SetColor(sf::Color::White);
		}
	}
	static_cast<LD32*>(m_game)->SetScore(m_score);
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
	m_next = root.get("next", "").asString(); 
	return true;
}
