#include "Slayer.hpp"
#include "Engine/Scene.hpp"
#include "Level.hpp"
#include <SFML/Window.hpp>

Slayer::Slayer(engine::Scene* scene): SpriteNode(scene), m_maxVelocity(20, 1), 
		m_velocityIncrease(5, 5), m_contactHandler(this), m_state(STANDING), m_weaponType(WT_NONE) {
	m_scene->OnContact.AddHandler(&m_contactHandler);
	static_cast<Level*>(scene)->SetSlayer(this);
}

Slayer::~Slayer() {
	m_scene->OnContact.RemoveHandler(&m_contactHandler);
}

void Slayer::OnUpdate(sf::Time interval) {
	auto vel =  m_body->GetLinearVelocity();
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && vel.x < m_maxVelocity.x) {
        m_body->ApplyLinearImpulse(b2Vec2(m_velocityIncrease.x * interval.asSeconds(), 0), m_body->GetWorldCenter(), true);
        if (m_flipped) {
            m_flipped = false;
            UpdateTexCoords();
        }
    }
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && vel.x > -m_maxVelocity.x) {
        m_body->ApplyLinearImpulse(b2Vec2(-m_velocityIncrease.x * interval.asSeconds(), 0), m_body->GetWorldCenter(), true);
        if (!m_flipped) {
            m_flipped = true;
            UpdateTexCoords();
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
}
void Slayer::PlayAni(std::string after) {
	std::string withwt;
	if (m_weaponType != WT_NONE) {
		withwt += FromWT(m_weaponType);
		withwt += "_";
	}
	switch (m_state) {
		case STANDING:
			withwt+="default";
			break;
		case RUNNING:
			withwt+="running";
			break;
		default:
			return;
			
	}
	PlayAnimation(withwt, after);
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