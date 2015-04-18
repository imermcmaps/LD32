#ifndef LD32_BAT_HPP
#define	LD32_BAT_HPP
#include "Enemy.hpp"
class Bat: public Enemy {
protected:
	sf::Vector2f m_targetPoint;
	float m_targetTime;
public:
	Bat(engine::Scene* scene);
	virtual ~Bat();
protected:
	virtual void OnUpdate(sf::Time interval);

};

#endif	/* LD32_BAT_HPP */

