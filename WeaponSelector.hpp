#ifndef LD32_WEAPONSELECTOR_HPP
#define	LD32_WEAPONSELECTOR_HPP
#include <Engine/Node.hpp>
#include <Engine/util/Event.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "misc.hpp"
#include "Level.hpp"

class WeaponSelector: public engine::Node {
	class KeyListener: public engine::util::EventHandler<const sf::Event::KeyEvent&> {
	protected:
		WeaponSelector* m_selector;
		
	public:
		KeyListener(WeaponSelector* ws): m_selector(ws) {} 
		void handle(const sf::Event::KeyEvent&);
	};
protected:
	WeaponType m_current;
	KeyListener m_keyListener;
	Level* m_level; // lazy, dont have to cast every time
	sf::Sound* m_disabledSound;
public:
	WeaponSelector(engine::Scene* scene);
	virtual ~WeaponSelector();
	void Select(WeaponType i);
private:

};

#endif	/* WEAPONSELECTOR_HPP */

