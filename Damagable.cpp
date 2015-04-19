#include "Damagable.hpp"
#include "Engine/util/Random.hpp"
#include "Engine/Factory.hpp"
#include "Level.hpp"
#include "Slayer.hpp"

Damagable::Damagable(engine::Scene* scene): SpriteNode(scene), m_health(10), 
		m_maxHealth(10), m_dead(false), m_hit(false), m_preCH(this), m_damage(2),
		m_invulnerable(false) {
	m_scene->OnContactPreSolve.AddHandler(&m_preCH);
}

Damagable::~Damagable() {
	m_scene->OnContactPreSolve.RemoveHandler(&m_preCH);
}
void Damagable::OnUpdate(sf::Time interval) {
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
bool Damagable::initialize(Json::Value& root) {
	if (!engine::SpriteNode::initialize(root)) {
		return false;
	}
	m_health = m_maxHealth = root.get("health", 10.0f).asFloat();
	m_damage = root.get("damage", 2.0f).asFloat();
	return true;
}
void Damagable::Damage(float damage){
	m_health-=damage;
	if (damage > 0.1 || m_health < 0){
		m_hit = true;
	}
	UpdateHealthbar();
}
void Damagable::UpdateHealthbar() {
	auto healthbar = GetChildByID("healthbar");
	if (healthbar) {
		auto health = healthbar->GetChildByID("health");
		if (m_health < 0 ){
			healthbar->SetActive(false);
		} else {
			auto orig = healthbar->GetSize();
			float ratio = m_health/m_maxHealth;
			orig.x = (orig.x -4)*ratio;
			health->SetSize(orig);
		}
	}
}

void Damagable::ContactHandler::handle(b2Contact* c, const b2Manifold* manifold) {
	void* udA = c->GetFixtureA()->GetBody()->GetUserData();
	void* udB = c->GetFixtureB()->GetBody()->GetUserData();
	Slayer* slayer = nullptr;
	if (udA == m_enemy && udB == static_cast<Level*>(m_enemy->GetScene())->GetSlayer()) {
		slayer = static_cast<Slayer*>(udB);
	} else if (udB == m_enemy && udA == static_cast<Level*>(m_enemy->GetScene())->GetSlayer()) {
		slayer = static_cast<Slayer*>(udA);
	}
	if (slayer) {
		slayer->Damage(m_enemy->GetDamage());
		c->SetEnabled(false);
	}
}
