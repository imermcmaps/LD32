#include "LD32.hpp"
#include <Engine/Factory.hpp>
#include <Engine/Scene.hpp>
#include "TitleScreen.hpp"

LD32::LD32() {
	m_windowTitle ="LD32 - Vampire Slayer Deluxe (c) imer.cc 2015";
	m_scene = engine::Factory::create<TitleScreen>("assets/scripts/title_menu.json", this);
	engine::Factory::CreateChildFromFile("assets/scripts/loading_bg.json", &m_loadingScene);
}

LD32::~LD32() {
}

void LD32::StartGame() {
	SwitchScene(engine::Factory::create<engine::Scene>("assets/scripts/level_1.json", this));
}