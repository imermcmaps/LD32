#include "SplashMessage.hpp"
#include <iostream>
auto easing = [](float time, float start, float change, float duration){
	return change*time/duration + start;
};
SplashMessage::SplashMessage(engine::Scene* scene): SpriteNode(scene), m_scaleTime(0) {
	
}

SplashMessage::~SplashMessage() {
}
void SplashMessage::OnDraw(sf::RenderTarget& target, sf::RenderStates states, float delta){
	const float fullCycle = 1; 
	m_scaleTime+=delta;
	if (m_scaleTime > fullCycle) {
		m_scaleTime = 0;
		m_scaleBack = !m_scaleBack;
	}
	float scale = easing(m_scaleBack?std::max(0.0f, fullCycle-m_scaleTime):m_scaleTime, 0.7f, 0.3f, fullCycle);
	SetSize(m_originalSize*scale);
	engine::SpriteNode::OnDraw(target, states, delta);
}
bool SplashMessage::initialize(Json::Value& root){
	if (!engine::SpriteNode::initialize(root)){
		return false;
	}
	m_originalSize = m_size;
	return true;
}