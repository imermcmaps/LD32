#ifndef LD32_ENEMY_HPP
#define	LD32_ENEMY_HPP
#include <Engine/SpriteNode.hpp>

#include "misc.hpp"
class Enemy: public engine::SpriteNode {
protected:
	float m_health;
	bool m_dead;
	bool m_hit;
public:
	Enemy(engine::Scene* scene);
	virtual ~Enemy();

	virtual uint8_t GetType() const {
		return NT_ENEMY;
	}
	void Damage(float damage);
	virtual void OnUpdate(sf::Time interval);

	virtual bool initialize(Json::Value& root);


private:

};

#endif	/* LD32_ENEMY_HPP */

