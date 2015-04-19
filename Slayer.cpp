#include "Slayer.hpp"
#include "Engine/Scene.hpp"
#include "Level.hpp"
#include "Engine/Factory.hpp"
#include <SFML/Window.hpp>
#include <Engine/Game.hpp>
#include <iostream>
#include <Engine/util/misc.hpp>
#include <limits>
Slayer::Slayer(engine::Scene* scene): Damagable(scene), m_maxVelocity(20, 1), 
		m_velocityIncrease(6, 10), m_contactHandler(this), m_state(STANDING), 
		m_weaponType(WT_NONE), m_weapon(nullptr), m_shootTime{}, m_invulnTime(0),
		m_respawnTimer(10.0f) {
	m_scene->OnContact.AddHandler(&m_contactHandler);
	static_cast<Level*>(scene)->SetSlayer(this);
	// Remove damagable enemy hit handler
	m_scene->OnContactPreSolve.RemoveHandler(&m_preCH);
	m_health = m_maxHealth = 100;
}

Slayer::~Slayer() {
	m_scene->OnContact.RemoveHandler(&m_contactHandler);
	// Add it back in so it can be properly removed by damagable destructor
	m_scene->OnContactPreSolve.AddHandler(&m_preCH);
}

void Slayer::OnUpdate(sf::Time interval) {
	if (m_dead) {
		m_respawnTimer-=interval.asSeconds();
		if (m_respawnTimer <= 0) {
			// Do this only once
			m_respawnTimer = std::numeric_limits<float>::infinity();
			static_cast<Level*>(m_scene)->Respawn();
			
		}
		return;
	}
	m_invulnTime-=interval.asSeconds();
	auto window = m_scene->GetGame()->GetWindow();
    sf::View v = window->getView();
	auto pos = GetGlobalPosition();
    v.setCenter(engine::util::minmax(v.getSize().x / 2, pos.x, m_scene->GetSize().x - v.getSize().x / 2), (engine::util::minmax(v.getSize().y / 2, pos.y, m_scene->GetSize().y - v.getSize().y / 2)));
    window->setView(v);
	auto vel =  m_body->GetLinearVelocity();
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && vel.x < m_maxVelocity.x) {
        m_body->ApplyLinearImpulse(b2Vec2(m_velocityIncrease.x * interval.asSeconds(), 0), m_body->GetWorldCenter(), true);
		if (!m_weapon) {
			SetFlipped(false);
		}
    }
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && vel.x > -m_maxVelocity.x) {
        m_body->ApplyLinearImpulse(b2Vec2(-m_velocityIncrease.x * interval.asSeconds(), 0), m_body->GetWorldCenter(), true);
		if (!m_weapon) {
			SetFlipped(true);
		}
    }
	if (m_contactHandler.GetCount() > 0) {
		m_jumpCooldown-=interval.asSeconds();
		if (m_jumpCooldown <= 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))) {
			m_jumpCooldown = 0.2f; 
			m_body->ApplyLinearImpulse(b2Vec2(0, -m_velocityIncrease.y), m_body->GetWorldCenter(), true);

		}
	}
	if ((vel.x < -0.1 || vel.x > 0.1)) {
		if (m_state != RUNNING) {
			m_state = RUNNING;
			PlayAni();
		}
	} else if (m_state != STANDING) {
		m_state = STANDING;
		PlayAni();
	}
	if (m_weapon) {
		// Weapon rotation
		auto delta = m_scene->GetGame()->GetMousePosition() - GetGlobalPosition();
		float angle = atan2(delta.y, delta.x);
		float rangle = angle;
		angle *= 180 / engine::util::fPI;
		SetFlipped(angle > 90 || angle < -90);
		if (weapons[m_weaponType].flip) {
			static_cast<engine::SpriteNode*>(m_weapon)->SetVFlipped(angle > 90 || angle < -90);
			//m_weapon->SetRotation(angle>90?angle-180:(angle< -90?angle+180:angle));
		}
		m_weapon->SetRotation(angle);
		
		// Shooting
		m_shootTime[m_weaponType] -= interval.asSeconds();
		if (m_shootTime[m_weaponType] < 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
			m_shootTime[m_weaponType] = weapons[m_weaponType].shootDelay;
			auto projectile = engine::Factory::CreateChildFromFile(weapons[m_weaponType].projectileFile, m_scene);
			auto pos = GetGlobalPosition();
			projectile->SetPosition(pos.x, pos.y);
			projectile->SetRotation(angle);
			auto body = projectile->GetBody();
			if (body) {
				auto vel = body->GetLinearVelocity();
				body->SetLinearVelocity(b2Vec2(vel.x*cosf(rangle), vel.y*sinf(rangle)));
			}
			static_cast<engine::SpriteNode*>(m_weapon)->PlayAnimation("fire", "default");
		}
	}
}
void Slayer::PlayAni(std::string after) {
	std::string ani;
	switch (m_state) {
		case STANDING:
			ani = "default";
			break;
		case RUNNING:
			ani = "running";
			break;
		default:
			return;
			
	}
	PlayAnimation(ani, after);
}

void Slayer::SetWeaponType(WeaponType weaponType) {
	auto w = GetChildByID(weapons[m_weaponType].name);
	if (w) {
		w->SetRender(false);
	}
	m_weaponType = weaponType;
	m_weapon = GetChildByID(weapons[m_weaponType].name);
	if (m_weapon) {
		m_weapon->SetRender(true);
	}

}
void Slayer::SetFlipped(bool flipped) {
	if (m_flipped == flipped) return;
	engine::SpriteNode::SetFlipped(flipped);
	const auto& size = GetSize();
	for (auto it = m_children.begin(); it != m_children.end(); it++) {
		auto pos = (*it)->GetPosition();
		(*it)->SetPosition(size.x-pos.x, pos.y);
	}
}
void Slayer::Damage(float damage) {
	if (damage <= 0 || m_invulnTime > 0) return;
	m_invulnTime = 0.5;
	m_health-=damage;
	if (m_health < 0) {
		if (!m_dead) {
			// TODO DEATHSOUND
			m_dead=true;
		}
	} else {
			// TODO HITSOUND
	}
	UpdateHealthbar();
}

void Slayer::ContactHandler::handle(b2Contact* contact, bool begin) {
	if (!contact->IsEnabled()) return;
	void* udA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* udB = contact->GetFixtureB()->GetBody()->GetUserData();
	if ((udA == m_slayer && contact->GetFixtureA()->GetType() == b2Shape::e_circle) 
		|| (udB == m_slayer  && contact->GetFixtureB()->GetType() == b2Shape::e_circle)) {
		if (static_cast<engine::Node*>(udA == m_slayer?udB:udA)->GetType()==NT_ENEMY) return;
		if (begin){
			m_count++;
		} else {
			m_count--;
		}
	}
}