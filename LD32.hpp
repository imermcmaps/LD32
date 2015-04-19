#ifndef LD32_HPP
#define	LD32_HPP

#include <Engine/Game.hpp>

#include "misc.hpp"

class LD32 : public engine::Game {
protected:
	bool m_unlocks[WT_MAX];
	int m_score;
public:
	LD32();
	virtual ~LD32();
	void StartGame();
	void Unlock(WeaponType wt);
	bool IsUnlocked(WeaponType wt);

    void SetScore(int score) {
    	m_score = score;
    }

    int GetScore() const {
    	return m_score;
    }
private:

};

#endif	/* LD32_HPP */

