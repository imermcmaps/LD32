#ifndef SLAYER_HPP
#define	SLAYER_HPP

#include "Damagable.hpp"
#include "misc.hpp"
#include "Engine/include/Engine/ResourceManager.hpp"


class Slayer: public Damagable {
	class ContactHandler: public engine::util::EventHandler<b2Contact*, bool> {
	protected:
		Slayer* m_slayer;
		size_t m_count;
		sf::Sound* m_upgradeSound;
	public:
		ContactHandler(Slayer* slayer): m_slayer(slayer), m_count(0) {
			m_upgradeSound = engine::ResourceManager::instance()->MakeSound("assets/sounds/upgrade.wav");
		}
		void handle(b2Contact*, bool);

        size_t GetCount() const {
        	return m_count;
        }
	};
protected:
	enum State {
		STANDING,
		RUNNING,
	};
	sf::Vector2f m_velocityIncrease;
	sf::Vector2f m_maxVelocity;
	float m_jumpCooldown;
	WeaponType m_weaponType;
	State m_state;
	Node* m_weapon;
	ContactHandler m_contactHandler;
	float m_shootTime[WT_MAX];
	float m_respawnTimer;

	sf::Sound* m_shootSound[WT_MAX];
public:
	Slayer(engine::Scene* scene);
	virtual ~Slayer();

    void SetWeaponType(WeaponType weaponType);
	
	void PlayAni(std::string after="");

	virtual void SetFlipped(bool flipped);
	virtual void Damage(float damage);
	
protected:
	virtual void OnUpdate(sf::Time interval);

};

#endif	/* SLAYER_HPP */

