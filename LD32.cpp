#include "LD32.hpp"
#include <Engine/Factory.hpp>
#include <Engine/Scene.hpp>
#include "TitleScreen.hpp"
#include "Level.hpp"

LD32::LD32(): m_unlocks{}, m_score(0) {
	m_windowTitle ="LD32 - Vampire Slayer Deluxe (c) imer.cc 2015";
	m_scene = engine::Factory::create<TitleScreen>("assets/scripts/title_menu.json", this);
	engine::Factory::CreateChildFromFile("assets/scripts/loading_bg.json", &m_loadingScene);
	//m_window.setVerticalSyncEnabled(true);
}

LD32::~LD32() {
}

void LD32::StartGame() {
	SwitchScene(engine::Factory::create<Level>("assets/scripts/level_2.json", this));
}
void LD32::Unlock(WeaponType wt){
	m_unlocks[wt] = true;
	if (m_scene) {
		auto selector = m_scene->GetUi()->GetChildByID("selector");
		if (selector) {
			auto s = static_cast<engine::SpriteNode*>(selector->GetChildByID(weapons[wt].name));
			if (s) {
				s->PlayAnimation("default");
			}
		}
	}
}
bool LD32::IsUnlocked(WeaponType wt) {
	return m_unlocks[wt];
}