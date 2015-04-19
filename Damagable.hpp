#ifndef LD32_ENEMY_HPP
#define	LD32_ENEMY_HPP
#include <Engine/SpriteNode.hpp>

#include "misc.hpp"
class Damagable: public engine::SpriteNode {
	class ContactHandler: public engine::util::EventHandler<b2Contact*, const b2Manifold*> {
		Damagable* m_enemy;
	public:
		ContactHandler(Damagable* p) : m_enemy(p) {
			
		}
		virtual void handle(b2Contact*, const b2Manifold* manifold);

	};
protected:
	float m_health;
	float m_maxHealth;
	float m_damage;
	bool m_dead;
	bool m_hit;
	ContactHandler m_preCH;
	bool m_invulnerable;
public:
	Damagable(engine::Scene* scene);
	virtual ~Damagable();

	virtual uint8_t GetType() const {
		return NT_ENEMY;
	}
	virtual void Damage(float damage);
	virtual void OnUpdate(sf::Time interval);

	virtual bool initialize(Json::Value& root);

    void SetDamage(float damage) {
    	m_damage = damage;
    }

    float GetDamage() const {
    	return m_damage;
    }
	void UpdateHealthbar();

private:

};

#endif	/* LD32_ENEMY_HPP */

