#ifndef LD32_PROJECTILE_HPP
#define	LD32_PROJECTILE_HPP
#include <Engine/SpriteNode.hpp>
#include "misc.hpp"

class Projectile: public engine::SpriteNode {
	class ContactHandler: public engine::util::EventHandler<b2Contact*, bool> {
		Projectile* m_projectile;
	public:
		ContactHandler(Projectile* p) : m_projectile(p) {
			
		}
		virtual void handle(b2Contact* contact, bool begin);

	};
protected:
	int m_hits;
	// hit in current update, dont allow multiple per
	bool m_hit;
	float m_damage;
	ContactHandler m_contactHandler;
	bool m_dead;
public:
	Projectile(engine::Scene* scene);
	virtual ~Projectile();
	void Hit(engine::Node*);
	
	virtual uint8_t GetType() const {
		return NT_PROJECTILE;
	};

	virtual bool initialize(Json::Value& root);
protected:
	virtual void OnUpdate(sf::Time interval);
};

#endif	/* PROJECTILE_HPP */

