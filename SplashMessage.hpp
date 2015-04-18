#ifndef LD32_SPLASHMESSAGE_HPP
#define	LD32_SPLASHMESSAGE_HPP
#include <Engine/SpriteNode.hpp>
class SplashMessage: public engine::SpriteNode {
protected:
	float m_scaleTime;
	bool m_scaleBack;
	sf::Vector2f m_originalSize;
public:
	SplashMessage(engine::Scene* scene);
	virtual ~SplashMessage();

	virtual bool initialize(Json::Value& root);
protected:
	virtual void OnDraw(sf::RenderTarget& target, sf::RenderStates states, float delta);
};

#endif	/* SPLASHMESSAGE_HPP */

