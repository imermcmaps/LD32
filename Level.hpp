#ifndef LD32_LEVEL_HPP
#define	LD32_LEVEL_HPP
#include <Engine/Scene.hpp>
class LD32;
class Slayer;
class Level: public engine::Scene {
protected:
	Slayer* m_slayer;
	bool m_initialized;
	bool m_paused;
public:
	Level(LD32* game);
	virtual ~Level();

    void SetSlayer(Slayer* slayer) {
    	m_slayer = slayer;
    }
	
    Slayer* GetSlayer() const {
    	return m_slayer;
    }
protected:
	virtual void update(sf::Time interval);
};

#endif	/* LD32_LEVEL_HPP */
