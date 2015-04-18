#ifndef LD32_LEVEL_HPP
#define	LD32_LEVEL_HPP
#include <Engine/Scene.hpp>
class LD32;
class Slayer;
class Level: public engine::Scene {
protected:
	Slayer* m_slayer;
public:
	Level(LD32* game);
	virtual ~Level();

    void SetSlayer(Slayer* slayer) {
    	m_slayer = slayer;
    }

    Slayer* GetSlayer() const {
    	return m_slayer;
    }
private:

};

#endif	/* LD32_LEVEL_HPP */

