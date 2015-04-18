#ifndef LD32_TITLESCREEN_HPP
#define	LD32_TITLESCREEN_HPP
#include <Engine/Scene.hpp>
class LD32;
class TitleScreen: public engine::Scene {
public:
	TitleScreen(LD32*);
	virtual ~TitleScreen();
	virtual bool initialize(Json::Value& root);
protected:



};

#endif	/* TITLESCREEN_H */

