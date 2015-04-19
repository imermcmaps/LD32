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
	int m_score;
	size_t m_enemies;
	sf::Vector2f m_respawnPoint;
	std::string m_next;
public:
	Level(LD32* game);
	virtual ~Level();

    void SetSlayer(Slayer* slayer) {
    	m_slayer = slayer;
    }
	
    Slayer* GetSlayer() const {
    	return m_slayer;
    }
	void IncEnemies() {
		m_enemies++;
	}
	void DecEnemies() {
		m_enemies--;
	}
	void ChangeScore(int amount);
	void Respawn();
	virtual bool initialize(Json::Value& root);
protected:
	virtual void update(sf::Time interval);
};

#endif	/* LD32_LEVEL_HPP */

