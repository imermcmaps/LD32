#include "LD32.hpp"
#include <Engine/Factory.hpp>
#include <Engine/Scene.hpp>
#include "TitleScreen.hpp"

LD32::LD32() {
	m_windowTitle ="LD32 - ???";
	m_scene = engine::Factory::create<TitleScreen>("assets/scripts/title_menu.json", this);
}

LD32::~LD32() {
}

void LD32::StartGame() {
	// TODO
}