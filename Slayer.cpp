#include "Slayer.hpp"
#include "Engine/Scene.hpp"
#include "Level.hpp"
#include "Engine/Factory.hpp"
#include <SFML/Window.hpp>
#include <Engine/Game.hpp>
#include <iostream>
#include <Engine/util/misc.hpp>
Slayer::Slayer(engine::Scene* scene): SpriteNode(scene), m_maxVelocity(20, 1), 
		m_velocityIncrease(5, 5), m_contactHandler(this), m_state(STANDING), 
		m_weaponType(WT_NONE), m_weapon(nullptr), m_shootTime(0.0f) {
	m_scene->OnContact.AddHandler(&m_contactHandler);
	static_cast<Level*>(scene)->SetSlayer(this);
}

Slayer::~Slayer() {
	m_scene->OnContact.RemoveHandler(&m_contactHandler);
}

void Slayer::OnUpdate(sf::Time interval) {
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
	} else if (m_state != STANDING && m_state != ATTACKING) {
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
		m_weapon->SetRotation(angle);
		
		// Shooting
		m_shootTime -= interval.asSeconds();
		if (m_shootTime < 0 && (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
			m_shootTime = weapons[m_weaponType].shootDelay;
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

void Slayer::ContactHandler::handle(b2Contact* contact, bool begin) {
	void* udA = contact->GetFixtureA()->GetBody()->GetUserData();
	void* udB = contact->GetFixtureB()->GetBody()->GetUserData();
	if ((udA == m_slayer && contact->GetFixtureA()->GetType() == b2Shape::e_circle) 
		|| (udB == m_slayer  && contact->GetFixtureB()->GetType() == b2Shape::e_circle)) {
		
		if (begin){
			m_count++;
		} else {
			m_count--;
		}
	}
}