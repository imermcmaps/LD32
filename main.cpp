#include "LD32.hpp"
#include <Engine/Factory.hpp>
#include "Slayer.hpp"
#include "WeaponSelector.hpp"
int main() {
	engine::Factory::RegisterType("slayer", engine::Factory::CreateChildNode<Slayer>);
	engine::Factory::RegisterType("selector", engine::Factory::CreateChildNode<WeaponSelector>);
	LD32 game;
	game.run();
	return 0;
}