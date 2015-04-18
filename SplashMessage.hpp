/* 
 * File:   SplashMessage.hpp
 * Author: iMer
 *
 * Created on 18. April 2015, 04:21
 */

#ifndef SPLASHMESSAGE_HPP
#define	SPLASHMESSAGE_HPP
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

