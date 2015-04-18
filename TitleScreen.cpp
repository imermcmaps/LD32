#include "TitleScreen.hpp"
#include <Engine/Factory.hpp>
#include <Engine/Button.hpp>
#include "SplashMessage.hpp"
#include "LD32.hpp"

TitleScreen::TitleScreen(LD32* game): Scene(game) {
	
}

TitleScreen::~TitleScreen() {
}
bool TitleScreen::initialize(Json::Value& root) {
	if (!engine::Scene::initialize(root)) {
		return false;
	}
	auto splash = engine::Factory::create<SplashMessage>("assets/scripts/splash.json", this);
	m_ui->AddNode(splash);
	splash = engine::Factory::create<SplashMessage>("assets/scripts/splash_bow.json", this);
	m_ui->AddNode(splash);
	auto button = engine::Factory::create<engine::Button>("assets/scripts/start_button.json", this);
	m_ui->AddNode(button);
	button->OnClick = [this] (engine::Button*, sf::Vector2f pos) {
		static_cast<LD32*>(m_game)->StartGame();
	};
	return true;
}


