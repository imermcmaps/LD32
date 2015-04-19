/* 
 * File:   Bat.cpp
 * Author: iMer
 * 
 * Created on 18. April 2015, 19:02
 */

#include "Bat.hpp"
#include "Level.hpp"
#include "Slayer.hpp"
#include "Engine/util/Random.hpp"
#include <iostream>

Bat::Bat(engine::Scene* scene): Damagable(scene), m_targetPoint(0,0) {
	m_deathSound = engine::ResourceManager::instance()->MakeSound("assets/sounds/bat_death.wav");
	m_hurtSound = engine::ResourceManager::instance()->MakeSound("assets/sounds/bat_hurt.wav");
}

Bat::~Bat() {
}
void Bat::OnUpdate(sf::Time interval) {
	Damagable::OnUpdate(interval);
	Level* level = static_cast<Level*>(m_scene);
	Slayer* slayer = level->GetSlayer();
	m_targetTime-=interval.asSeconds();
	if (m_targetTime <= 0) {
		engine::util::RandomFloat r(0, 1);
		m_targetTime = static_cast<float>(0.4f * (r()+1));
		m_targetPoint = (slayer->GetGlobalPosition());
		if (r() > 0.4) {
			m_targetPoint.y -= static_cast<float>(slayer->GetSize().y*(r()+0.5f));
		}
		if (r() > 0.4) {
			m_targetPoint.x -= static_cast<float>(slayer->GetSize().x*5*(r()+0.5f));
		}
		if (r() > 0.4) {
			m_targetPoint.x += static_cast<float>(slayer->GetSize().x*5*(r()+0.5f));
		}
	}
	auto delta = m_targetPoint-GetGlobalPosition();
	float dist = sqrtf(delta.x*delta.x+delta.y*delta.y);
	if (dist < 700 && abs(delta.y) < 300) {
		float angle = atan2(delta.y, delta.x);

		const float v = 3 * interval.asSeconds() ;
		m_body->ApplyLinearImpulse(b2Vec2(v * cosf(angle), v * sinf(angle)), m_body->GetWorldCenter(), true);
	}
}


