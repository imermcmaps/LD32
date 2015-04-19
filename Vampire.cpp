/* 
 * File:   Vampire.cpp
 * Author: iMer
 * 
 * Created on 18. April 2015, 19:02
 */

#include "Vampire.hpp"
#include "Level.hpp"
#include "Slayer.hpp"
#include "Engine/util/Random.hpp"
#include <iostream>

Vampire::Vampire(engine::Scene* scene): Damagable(scene), m_targetPoint(0,0) {
	
}

Vampire::~Vampire() {
}
void Vampire::OnUpdate(sf::Time interval) {
	Damagable::OnUpdate(interval);
	Level* level = static_cast<Level*>(m_scene);
	Slayer* slayer = level->GetSlayer();
	m_targetTime-=interval.asSeconds();
	engine::util::RandomFloat r(0, 1);
	if (m_targetTime <= 0) {
		m_targetTime = static_cast<float>(0.4f * (r()+1));
		m_targetPoint = (slayer->GetGlobalPosition());
	}
	auto delta = m_targetPoint-GetGlobalPosition();
	if (abs(delta.x) < 500 && abs(delta.x) > 10) {
		const float v = 2 * interval.asSeconds() ;
		m_body->ApplyLinearImpulse(b2Vec2(v * (delta.x > 0?1:-1), r() < 0.005?-5:0), m_body->GetWorldCenter(), true);
	}
}


