/* 
 * File:   Enemy.cpp
 * Author: iMer
 * 
 * Created on 18. April 2015, 18:35
 */

#include "Enemy.hpp"
#include "Engine/util/Random.hpp"
#include "Engine/Factory.hpp"

Enemy::Enemy(engine::Scene* scene): SpriteNode(scene), m_health(10), m_dead(false), m_hit(false) {
}

Enemy::~Enemy() {
}
void Enemy::OnUpdate(sf::Time interval) {
	if (m_hit) {
		m_hit=false;
		engine::util::RandomFloat r(0, 1);
		const int c = 5*(r()*10+1);
		for (size_t i =0; i<c;i++) {
			auto particle = engine::Factory::CreateChildFromFile("assets/scripts/blood_particle.json", m_scene);
			auto pos = GetGlobalPosition();
			particle->SetPosition(pos.x, pos.y);
			if (particle->GetBody()) {
				particle->GetBody()->SetLinearVelocity(b2Vec2(10*(r()-0.5), 10*(r()-0.5)));
			}
		}
	}
	if (m_health <= 0 && !m_dead) {
		m_dead = true;
		if (GetBody()) {
			GetBody()->SetActive(false);
		}
		PlayAnimation("death");
		if (GetAnimation()) {
			GetAnimation()->OnOver = [this](){
				Delete();
			};
		}
	}
}
bool Enemy::initialize(Json::Value& root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_health = root.get("health", 10.0f).asFloat();
	return true;
}
void Enemy::Damage(float damage){

	m_hit = true;
	m_health-=damage;
}