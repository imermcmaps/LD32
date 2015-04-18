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

Bat::Bat(engine::Scene* scene): Enemy(scene), m_targetPoint(0,0) {
	
}

Bat::~Bat() {
}
void Bat::OnUpdate(sf::Time interval) {
	Enemy::OnUpdate(interval);
	Level* level = static_cast<Level*>(m_scene);
	Slayer* slayer = level->GetSlayer();
	m_targetTime-=interval.asSeconds();
	if (m_targetTime <= 0) {
		engine::util::RandomFloat r(0, 1);
		m_targetTime = 0.4f * (r()+1);
		m_targetPoint = (slayer->GetGlobalPosition());
		if (r() > 0.4) {
			m_targetPoint.y -= slayer->GetSize().y*(r()+0.5f);
		}
		if (r() > 0.4) {
			m_targetPoint.x -= slayer->GetSize().x*5*(r()+0.5f);
		}
		if (r() > 0.4) {
			m_targetPoint.x += slayer->GetSize().x*5*(r()+0.5f);
		}
	}
	auto delta = m_targetPoint-GetGlobalPosition();
	float dist = sqrtf(delta.x*delta.x+delta.y*delta.y);
	if (dist < 700) {
		float angle = atan2(delta.y, delta.x);

		const float v = 3 * interval.asSeconds() ;
		m_body->ApplyLinearImpulse(b2Vec2(v * cosf(angle), v * sinf(angle)), m_body->GetWorldCenter(), true);
	}
}


