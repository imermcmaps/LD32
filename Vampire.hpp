#ifndef LD32_VAMPIRE_HPP
#define	LD32_VAMPIRE_HPP
#include "Damagable.hpp"
class Vampire: public Damagable {
protected:
	sf::Vector2f m_targetPoint;
	float m_targetTime;
public:
	Vampire(engine::Scene* scene);
	virtual ~Vampire();
protected:
	virtual void OnUpdate(sf::Time interval);

};

#endif	/* LD32_VAMPIRE_HPP */

