/* 
 * File:   Enemy.cpp
 * Author: iMer
 * 
 * Created on 18. April 2015, 18:35
 */

#include "Enemy.hpp"
#include "Engine/util/Random.hpp"
#include "Engine/Factory.hpp"
#include "Level.hpp"
#include "Slayer.hpp"

Enemy::Enemy(engine::Scene* scene): SpriteNode(scene), m_health(10), m_dead(false), m_hit(false) {
}

Enemy::~Enemy() {
}
void Enemy::OnUpdate(sf::Time interval) {
	if (m_hit) {
		m_hit=false;
		engine::util::RandomFloat r(0, 1);
		const size_t c = static_cast<size_t>(2*(r()*10+1));
		for (size_t i =0; i<c;i++) {
			auto particle = engine::Factory::CreateChildFromFile("assets/scripts/blood_particle.json", m_scene);
			auto pos = GetGlobalPosition();
			particle->SetPosition(pos.x, pos.y);
			if (particle->GetBody()) {
				particle->GetBody()->SetLinearVelocity(b2Vec2(static_cast<float>(10*(r()-0.5f)), static_cast<float>(10*(r()-0.5f))));
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
	m_health-=damage;
	if (damage > 0.1 || m_health < 0){
		m_hit = true;
	}
}
void Enemy::ContactHandler::handle(b2Contact* c, const b2Manifold* manifold) {
	void* udA = c->GetFixtureA()->GetBody()->GetUserData();
	void* udB = c->GetFixtureB()->GetBody()->GetUserData();
	Slayer* slayer = nullptr;
	if (udA == m_enemy && udB == static_cast<Level*>(m_enemy->GetScene())->GetSlayer()) {
		slayer = static_cast<Slayer*>(udB);
	} else if (udB == m_enemy && udA == static_cast<Level*>(m_enemy->GetScene())->GetSlayer()) {
		slayer = static_cast<Slayer*>(udA);
	}
	if (slayer) {
//		slayer->Damage(m_enemy);
		c->SetEnabled(false);
	}
}
