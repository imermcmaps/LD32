#include "Projectile.hpp"
#include "misc.hpp"
#include "Enemy.hpp"
#include "Engine/Scene.hpp"

Projectile::Projectile(engine::Scene* scene): SpriteNode(scene), m_hits(-1), 
		m_damage(4), m_contactHandler(this), m_dead(false) {
	m_scene->OnContact.AddHandler(&m_contactHandler);
}

Projectile::~Projectile() {
	m_scene->OnContact.RemoveHandler(&m_contactHandler);
}

void Projectile::Hit(engine::Node* node) {
	if (!m_hit) {
		m_hits--;
		m_hit=true;
		PlayAnimation("hit", "default");
	}
	if (node->GetType() != NT_ENEMY) {
		return;
	}
	Enemy* enemy = static_cast<Enemy*>(node);
	enemy->Damage(m_damage);
}

bool Projectile::initialize(Json::Value& root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_damage = root.get("damage", 4).asFloat();
	m_hits = root.get("hits", -1).asInt();
	auto it = m_animations.find("end");
	if (it != m_animations.end()) {
		it->second->OnOver = [this](){
			Delete();
		};
	}
	it = m_animations.find("default");
	if (it != m_animations.end() && it->second->GetSpeed() != 0) {
		it->second->OnOver = [this](){
			Delete();
		};
	}
	return true;
}
void Projectile::OnUpdate(sf::Time interval) {
	if (m_hits == 0 && !m_dead) {
		m_dead=true;
		GetBody()->SetActive(false);
		PlayAnimation("end");
		return;
	}
	m_hit = false;
}

void Projectile::ContactHandler::handle(b2Contact* contact, bool begin) {
	void* udA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* udB = contact->GetFixtureB()->GetBody()->GetUserData();
	engine::Node* other = nullptr;
	if (udA == m_projectile) {
		other = static_cast<engine::Node*>(udB);
	} else if (udB == m_projectile) {
		other = static_cast<engine::Node*>(udA);
	}
	if (other) {
		m_projectile->Hit(other);
	}
}
