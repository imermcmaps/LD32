#include "WeaponSelector.hpp"
#include "Engine/SpriteNode.hpp"
#include "Engine/Game.hpp"
#include "Slayer.hpp"
#include <SFML/Window.hpp>
#include <Engine/ResourceManager.hpp>
#include <iostream>

WeaponSelector::WeaponSelector(engine::Scene* scene): Node(scene), m_current(WT_NONE),
		m_level(static_cast<Level*>(scene)), m_keyListener(this) {
	m_disabledSound = engine::ResourceManager::instance()->MakeSound("assets/sounds/disabled.ogg");
	m_scene->GetGame()->OnKeyDown.AddHandler(&m_keyListener);
}

WeaponSelector::~WeaponSelector() {
	m_scene->GetGame()->OnKeyDown.RemoveHandler(&m_keyListener);
	delete m_disabledSound;
}

void WeaponSelector::Select(WeaponType wt) {
	engine::Node* nchild = GetChildByID(weapons[wt].name);
	if (!nchild || nchild->GetType() != engine::NT_SPRITE) {
		return;
	}
	engine::SpriteNode* child = static_cast<engine::SpriteNode*>(nchild);
	if (child->GetAnimationName() == "disabled") {
		m_disabledSound->play();
		return;
	}
	engine::Node* noldChild = GetChildByID(weapons[m_current].name);
	if (noldChild && noldChild->GetType() == engine::NT_SPRITE) {
		static_cast<engine::SpriteNode*>(noldChild)->PlayAnimation("default");
	}
	m_current = wt;
	child->PlayAnimation("active");
	m_level->GetSlayer()->SetWeaponType(wt);
}

void WeaponSelector::KeyListener::handle(const sf::Event::KeyEvent& e) {
	switch (e.code) {
		case sf::Keyboard::Num1:
			m_selector->Select(WT_NONE);
			break;
		case sf::Keyboard::Num2:
			m_selector->Select(WT_CROSSBOW);
			break;
		case sf::Keyboard::Num3:
			m_selector->Select(WT_LAUNCHER);
			break;
		case sf::Keyboard::Num4:
			m_selector->Select(WT_SUNCANNON);
			break;
		
	}
}